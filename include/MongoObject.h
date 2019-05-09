#ifndef CHINET_MONGOOBJECT_H
#define CHINET_MONGOOBJECT_H

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <memory>
#include <cmath>

#include <mongoc.h>

#include "Functions.h"


class MongoObject{

private:

    bool is_connected_to_db_;

    mongoc_uri_t *uri;
    mongoc_client_t *client;
    bson_error_t error;
    mongoc_collection_t *collection;


protected:

    bson_t document;

    std::string uri_string;
    std::string db_string;
    std::string app_string;
    std::string collection_string;

    bson_oid_t oid_document;
    bson_oid_t oid_precursor;
    uint64_t time_of_death;

    template<typename T>
    T get_value(const char* key){
        T v(0);
        bson_iter_t iter;
        if(std::is_same<T, int>::value) {
            if (bson_iter_init_find(&iter, &document, key) &&
                (BSON_ITER_HOLDS_INT64(&iter))) {
                return (int) bson_iter_int64(&iter);
            }
        }
        else if(std::is_same<T, double>::value) {
            if (bson_iter_init_find(&iter, &document, key) &&
                (BSON_ITER_HOLDS_DOUBLE(&iter))) {
                return bson_iter_double(&iter);
            }
        }
        else if(std::is_same<T, bool>::value) {
            if (bson_iter_init_find(&iter, &document, key) &&
                (BSON_ITER_HOLDS_BOOL(&iter))) {
                return bson_iter_bool(&iter);
            }
        }
        return v;
    }

    template <typename T>
    void set_value(const char* key, T value){
        bson_iter_t iter;
        if(std::is_same<T, bool>::value) {
            if (bson_iter_init_find(&iter, &document, key) &&
                BSON_ITER_HOLDS_BOOL(&iter)) {
                bson_iter_overwrite_bool(&iter, value);
            }
        }
        else if(std::is_same<T, double>::value) {
            if (bson_iter_init_find(&iter, &document, key) &&
                BSON_ITER_HOLDS_DOUBLE(&iter)) {
                bson_iter_overwrite_double(&iter, value);
            }
        }
        else if(std::is_same<T, int>::value) {
            if (bson_iter_init_find(&iter, &document, key) &&
                BSON_ITER_HOLDS_INT64(&iter)) {
                bson_iter_overwrite_int64(&iter, value);
            }
        }
    }

    void set_value(const char* key, bson_oid_t value);

    template <typename T>
    std::vector<T> get_array(const char* key){
        bson_iter_t iter;
        std::vector<T> v{};

        // double
        if(std::is_same<T, double>::value){
            if (bson_iter_init_find(&iter, &document, key) &&
                (BSON_ITER_HOLDS_ARRAY(&iter))){
                bson_iter_t iter_array;
                bson_iter_recurse (&iter, &iter_array);
                while (bson_iter_next (&iter_array) &&
                       BSON_ITER_HOLDS_DOUBLE(&iter_array)) {
                    v.push_back(bson_iter_double(&iter_array));
                }
            }
            return v;
        }
        // int
        else if(std::is_same<T, int>::value){
            if (bson_iter_init_find(&iter, &document, key) &&
                (BSON_ITER_HOLDS_ARRAY(&iter))) {
                bson_iter_t iter_array;
                bson_iter_recurse(&iter, &iter_array);
                while (bson_iter_next(&iter_array) &&
                       BSON_ITER_HOLDS_INT64(&iter_array)) {
                    v.push_back((int) bson_iter_int64(&iter_array));
                }
            }
            return v;
        }
        return v;
    }

    template <typename T>
    void set_array(const char* key, T &value){
        bson_t src = MongoObject::get_bson();
        bson_t dst; bson_init (&dst);
        bson_copy_to_excluding_noinit(&src, &dst,
                                      key,
                                      NULL
        );
        append_number_array(&dst, key, value);
        bson_copy_to(&dst, &document);
    }

    static bool string_to_oid(const std::string &oid_string, bson_oid_t *oid);

    template <typename T>
    void create_oid_dict_in_doc(
            bson_t *doc,
            std::string key,
            const std::map<std::string, std::shared_ptr<T>> &mongo_obj_array){

        bson_t child;
        bson_append_document_begin(doc, key.c_str(), key.size(), &child);
        for(auto &v : mongo_obj_array){
            const bson_oid_t b = v.second->get_oid();
            bson_append_oid(&child, v.first.c_str(), v.first.size(), &b);
        }
        bson_append_document_end(doc, &child);
    }

    template <typename T>
    void append_number_array(bson_t *doc, std::string key, T &values){
        bson_t child;
        bson_append_array_begin(doc, key.c_str(), key.size(), &child);
        for(auto &v : values){
            bson_append_double(&child, "", 0, (double) v);
        }
        bson_append_array_end(doc, &child);
    }

    template <typename T>
    void create_oid_array_in_doc(
            bson_t *doc,
            std::string target_field_name,
            const std::set<std::shared_ptr<T>> &mongo_obj_array);

    template <typename T>
    bool create_and_connect_objects_from_oid_doc(
            const bson_t *doc,
            const char *document_name,
            std::map<std::string, std::shared_ptr<T>> *target_map){
        bool return_value = true;

        bson_iter_t iter;
        bson_iter_t child;
        if (bson_iter_init_find (&iter, doc, document_name) &&
            BSON_ITER_HOLDS_DOCUMENT (&iter) &&
            bson_iter_recurse (&iter, &child)) {
            while (bson_iter_next (&child)) {
                if (BSON_ITER_HOLDS_OID(&child)){
                    // read oid
                    bson_oid_t new_oid;
                    bson_oid_copy(bson_iter_oid(&child), &new_oid);
                    // create new obj
                    auto o = std::make_shared<T>();
                    // connect obj to db
                    return_value &= connect_object_to_db(o);
                    // read obj from db
                    o->read_from_db(oid_to_string(new_oid));
                    std::string key = bson_iter_key(&child);
                    // add obj to the target map
                    target_map->insert(std::pair<std::string, std::shared_ptr<T>>(key, o));
                }
            }
        } else{
            std::cerr << "Error: no nodes section in Session" << std::endl;
            return_value &= false;
        }
        return return_value;
    }

    template <typename T>
    bool create_and_connect_objects_from_oid_array(
            const bson_t *doc,
            const char *array_name,
            std::set<std::shared_ptr<T>> *target_set);

    static void append_string(bson_t *dst, std::string key, std::string content);

    static const std::string get_string_by_key(bson_t *doc, std::string key);

public:

    MongoObject();
    ~MongoObject();

    /// Connects the instance of @class MongoObject to a database
    /*!
     *
     * @param uri_string
     * @param db_string
     * @param app_string
     * @param collection_string
     * @return
     */
    bool connect_to_db(
            const std::string &uri_string,
            const std::string &db_string,
            const std::string &app_string,
            const std::string &collection_string
    );

    template <typename T>
    bool connect_object_to_db(T o){
        return o->connect_to_db(
                uri_string,
                db_string,
                app_string,
                collection_string
        );
    }

    /// Disconnects the @class MongoObject instance from the DB
    void disconnect_from_db();

    /// Returns true if the instance of the @class MongoObject is connected to the DB
    bool is_connected_to_db();

    static std::string oid_to_string(bson_oid_t oid){
        char oid_str[25];
        bson_oid_to_string(&oid, oid_str);
        return std::string(oid_str, 25);
    }

    /// Writes a BSON document to the connected MongoDB
    /*!
     *
     * @param doc a pointer to the BSON document that is written to the MongoDB
     * @param write_option integer specifying the write mode - 1: replaces the document (no upsert), 2: insert
     * the document das a new document, default: updates an existing document.
     *
     * @return true in case of a successful write.
     */
    bool write_to_db(const bson_t &doc, int write_option = 0);

    virtual bool write_to_db();

    std::string create_copy();

    virtual bool read_from_db(const std::string &oid_string);

    virtual bool read_from_db();

    virtual bson_t get_bson();
    virtual bson_t get_bson_excluding(...);

    std::string get_json();

    std::string get_oid_string();

    bson_oid_t get_oid();

    const bson_t* get_document();

    bool operator==(MongoObject const& b){
        return (
                bson_oid_equal(&b.oid_document, &oid_document) &&
                (uri_string == b.uri_string)
        );
    };

};

#endif //CHINET_MONGOOBJECT_H
