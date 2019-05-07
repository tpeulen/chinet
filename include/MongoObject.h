//
// Created by thomas on 5/1/19.
//

#ifndef CHINET_MONGOOBJECT_H
#define CHINET_MONGOOBJECT_H

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <memory>
#include <cmath>

#include <mongoc.h>


class MongoObject {

private:
    bool is_connected_to_db_;

    mongoc_uri_t *uri;
    mongoc_client_t *client;
    bson_error_t error;
    mongoc_collection_t *collection;

    // TODO: use buffers to minimize BSON access
    std::map<std::string, double> buff_double_;
    std::map<std::string, uint64_t> buff_int64_;
    std::map<std::string, bool> buff_bool_;

    std::map<std::string, std::vector<double>> buff_double_vector_;
    std::map<std::string, std::vector<uint64_t>> buff_int64_vector_;
    std::map<std::string, std::vector<bool>> buff_bool_vector_;

    std::map<std::string, std::string> buff_string_;

protected:
    bson_t document;

    std::string uri_string;
    std::string db_string;
    std::string app_string;
    std::string collection_string;

    bson_oid_t oid_document;
    bson_oid_t oid_precursor;
    uint64_t time_of_death;

    static bool string_to_oid(const std::string &oid_string, bson_oid_t *oid){
        if (bson_oid_is_valid(oid_string.c_str(), oid_string.size())) {
            // convert the oid string to an oid
            bson_oid_init_from_string(oid, oid_string.c_str());
            return true;
        } else{
            bson_oid_init(oid, nullptr);
            std::cerr << "OID string not valid." << std::endl;
            return false;
        }
    }

    void overwrite_oid_in_field(bson_oid_t* oid, std::string field){
        bson_iter_t iter;
        if (bson_iter_init_find(&iter, get_document(), field.c_str()) &&
            BSON_ITER_HOLDS_OID(&iter)) {
            bson_iter_overwrite_oid(&iter, oid);
        }
    }

    template <typename T>
    void create_oid_dict_in_doc(
            bson_t *doc,
            std::string target_field_name,
            const std::map<std::string, std::shared_ptr<T>> &mongo_obj_array){

        bson_t child;
        bson_append_document_begin(doc, target_field_name.c_str(), target_field_name.size(), &child);
        for(auto &v : mongo_obj_array){
            const bson_oid_t* b = v.second->get_oid();
            bson_append_oid(&child, v.first.c_str(), v.first.size(), b);
        }
        bson_append_document_end(doc, &child);
    }

    template <typename T>
    void create_oid_array_in_doc(
            bson_t *doc,
            std::string target_field_name,
            const std::set<std::shared_ptr<T>> &mongo_obj_array){

        bson_t child;
        bson_append_array_begin(doc, target_field_name.c_str(), target_field_name.size(), &child);
        for(auto &v : mongo_obj_array){
            const bson_oid_t* b = v->get_oid();
            bson_append_oid(&child, "", 0, b);
        }
        bson_append_array_end(doc, &child);
    }

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
            std::set<std::shared_ptr<T>> *target_set){
        bool return_value = true;

        bson_iter_t iter;
        bson_iter_t child;
        if (bson_iter_init_find (&iter, doc, array_name) &&
            BSON_ITER_HOLDS_ARRAY (&iter) &&
            bson_iter_recurse (&iter, &child)) {
            while (bson_iter_next (&child)) {
                if (BSON_ITER_HOLDS_OID(&child)){
                    // read oid
                    bson_oid_t new_oid;
                    bson_oid_copy(bson_iter_oid(&child), &new_oid);

                    // create new obj
                    std::shared_ptr<T> o = std::make_shared<T>();

                    // connect obj to db
                    return_value &= connect_object_to_db(o);

                    // read obj from db

                    o->read_from_db(oid_to_string(new_oid));

                    // add obj to the target set
                    target_set->insert(o);
                }
            }
        } else{
            std::cerr << "Error: no nodes section in Session" << std::endl;
            return_value &= false;
        }
        return return_value;
    }

    static void append_string(bson_t *dst, const std::string key, const std::string content){
        bson_append_utf8(dst, key.c_str(), key.size(), content.c_str(), content.size());
    }

    static const std::string get_string_by_key(bson_t *doc, const std::string key){
        bson_iter_t iter;

        if (bson_iter_init(&iter, doc) &&
            bson_iter_find(&iter, key.c_str()) &&
            BSON_ITER_HOLDS_UTF8(&iter)) {
            const char* str;
            uint32_t len;
            str = bson_iter_utf8(&iter, &len);
            return std::string(str, len);
        }
        std::cerr << "Error: the key does not contain an string" << std::endl;
        return "";
    }

    const double get_double(const char* key){
        try {
            return buff_double_.at(key);
        }
        catch (std::out_of_range &o){
            double val;
            bson_iter_t iter;
            if (bson_iter_init_find (&iter, &document, key) &&
                BSON_ITER_HOLDS_DOUBLE(&iter)) {
                val = bson_iter_double(&iter);
            } else{
                std::cerr << "Error: the key does not contain an double" << std::endl;
                val = NAN;
            }
            buff_double_[key] = val;
            return val;
        }
    }

    const std::vector<double> get_double_vector(const char* key){
        try {
            return buff_double_vector_.at(key);
        }
        catch (std::out_of_range &o){
            std::vector<double> val;
            bson_iter_t iter;
            if (bson_iter_init(&iter, &document) &&
                bson_iter_find(&iter, key) &&
                BSON_ITER_HOLDS_ARRAY(&iter)) {
                bson_iter_t iter_array;
                bson_iter_recurse (&iter, &iter_array);
                while (bson_iter_next (&iter_array)) {
                    val.push_back(bson_iter_double(&iter_array));
                }
            } else{
                std::cerr << "Error: the key does not contain an array" << std::endl;
            }
            buff_double_vector_[key] = val;
            return val;
        }
    }

    const int64_t get_int64(const char* key){
        try {
            return buff_int64_.at(key);
        }
        catch (std::out_of_range &o){
            int64_t val;
            bson_iter_t iter;
            if (bson_iter_init_find (&iter, &document, key) &&
                BSON_ITER_HOLDS_INT64(&iter)) {
                val = bson_iter_int64(&iter);
            } else{
                std::cerr << "Error: the key does not contain an int64" << std::endl;
                val = 0;
            }
            buff_int64_[key] = val;
            return val;
        }
    }

    const bool get_bool(const char* key){
        bson_iter_t iter;
        if (bson_iter_init_find (&iter, &document, key) &&
            BSON_ITER_HOLDS_BOOL(&iter)) {
            return bson_iter_bool(&iter);
        } else{
            std::cerr << "Error: the key does not contain an bool" << std::endl;
            return true;
        }
    }

    const void set_key(const char *key, double v){
        buff_double_[key] = v;
    }

    const void set_key(const char *key, std::vector<double> v){
        buff_double_vector_[key] = v;
    }

    const void set_key(const char *key, int64_t v){
        buff_int64_[key] = v;
    }

    const void set_key(const char *key, int v){
        buff_int64_[key] = v;
    }

    const void set_key(const char *key, bool v){
        bson_iter_t iter;
        if (bson_iter_init_find(&iter, &document, key) &&
            BSON_ITER_HOLDS_BOOL(&iter)) {
            bson_iter_overwrite_bool(&iter, v);
        }
    }

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

    bson_oid_t* get_oid();

    const bson_t* get_document();

    bool operator==(MongoObject const& b){
        return (
                bson_oid_equal(&b.oid_document, &oid_document) &&
                (uri_string == b.uri_string)
        );
    };

};


#endif //CHINET_MONGOOBJECT_H
