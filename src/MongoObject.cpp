#include "MongoObject.h"


MongoObject::MongoObject():
uri_string(""),
db_string(""),
app_string(""),
collection_string(""),
time_of_death(0),
is_connected_to_db_(false)
{
    bson_oid_init(&oid_document, nullptr);
    bson_oid_copy(&oid_document, &oid_precursor);
    uri = nullptr;
    client = nullptr;
    collection = nullptr;

    bson_t *doc = BCON_NEW(
            "_id", BCON_OID(&oid_document),
            "precursor", BCON_OID(&oid_document),
            "death", BCON_INT64(time_of_death)
    );

    bson_init(&document);
    bson_copy_to(doc, &document);

}

MongoObject::~MongoObject() {
    time_of_death = Functions::get_time();
    std::clog << "Time of death: " << time_of_death << std::endl;
    write_to_db();
    disconnect_from_db();
}

bool MongoObject::connect_to_db(
        const std::string &uri_string,
        const std::string &db_string,
        const std::string &app_string,
        const std::string &collection_string
){
    this->uri_string = uri_string;
    this->db_string = db_string;
    this->app_string = app_string;
    this->collection_string = collection_string;

    mongoc_init();

    // Database
    //----------------------------------------------------------------
    std::clog << "connecting to:" << uri_string  << std::endl;
    uri = mongoc_uri_new_with_error (uri_string.c_str(), &error);
    if (!uri) {
        std::cerr << "failed to parse URI:" << uri_string << std::endl;
        std::cerr << "error message:       " << error.message << std::endl;
        return false;
    } else{
        /*
        * Create a new client instance
        */
        client = mongoc_client_new_from_uri (uri);
        /*
        * Register the application name so we can track it in the profile logs
        * on the server. This can also be done from the URI (see other examples).
        */
        mongoc_client_set_appname (client, app_string.c_str());
        /*
        * Get a handle on the collection
        */
        collection = mongoc_client_get_collection (client, db_string.c_str(), collection_string.c_str());
        is_connected_to_db_ = true;
        return true;
    }
}

void MongoObject::disconnect_from_db() {
    if(is_connected_to_db()){
        // destroy cursor, collection, session before the client they came from
        if (collection) {
            mongoc_collection_destroy (collection);
        }
        if (uri) {
            mongoc_uri_destroy (uri);
        }
        if (client) {
            mongoc_client_destroy (client);
        }
        mongoc_cleanup ();
    }
    is_connected_to_db_ = false;
}

bool MongoObject::write_to_db(const bson_t &doc, int write_option) {
    bool return_value = false;

    if(is_connected_to_db()) {
        return_value = true;

        bson_t *query = nullptr;
        bson_t *update = nullptr;
        bson_t reply;

        query = BCON_NEW ("_id", BCON_OID(&oid_document));

        bson_t *opts = BCON_NEW(
                "upsert", BCON_BOOL(true)
        );

        switch (write_option){

            case 1:
                // option 1 - write as an replacement
                if(!mongoc_collection_replace_one(collection, query, &doc, nullptr, &reply, &error))
                {
                    std::cerr << error.message;
                    return_value &= false;
                }
                break;

            case 2:
                // option 2 - insert as an new document
                if(!mongoc_collection_insert_one(collection, &doc, nullptr, &reply, &error))
                {
                    std::cerr << error.message;
                    return_value &= false;
                }
                break;

            default:
                // option 0 - write as an update
                update = BCON_NEW ("$set", BCON_DOCUMENT(&doc));
                if(!mongoc_collection_find_and_modify(
                        collection,
                        query,
                        nullptr,
                        update,
                        nullptr,
                        false,
                        true, // update by upsert: if the oid is not in the DB create a new document
                        false,
                        &reply, &error)
                        )
                {
                    std::cerr << error.message;
                    return_value &= false;
                }
                break;
        }

        // destroy
        bson_destroy(update);
        bson_destroy(query);
        bson_destroy(&reply);

    } else{
        std::cerr << "Not connected: cannot write" << std::endl;
    }

    return return_value;
}

bool MongoObject::write_to_db() {
    return write_to_db(
            get_bson(), 0
    );
}

bool MongoObject::read_from_db(const std::string &oid_string){
    bson_oid_t oid;
    if (string_to_oid(oid_string, &oid)) {

        if(!is_connected_to_db()){
            std::cerr << "Not connected to a DB." << std::endl;
            return false;
        } else{
            // find the oid in the collection
            bson_t *query = nullptr;
            query = BCON_NEW ("_id", BCON_OID(&oid));

            size_t len;
            std::clog << "reading: " << bson_as_json(query, &len) << std::endl;

            mongoc_cursor_t *cursor; // cursor pointing to the new document
            cursor = mongoc_collection_find_with_opts(
                    collection,
                    query,
                    nullptr, // the opts
                    nullptr  // the read_prefs
            );

            const bson_t *doc;
            while (mongoc_cursor_next (cursor, &doc)) {
                std::clog << "read content: " << bson_as_json(doc, &len) << std::endl;

                bson_reinit(&document);
                bson_copy_to(doc, &document);

                // Copy the data from the document to the object's attributes
                bson_oid_copy(&oid, &oid_document);

                bson_iter_t iter;
                // oid_precursor
                if (bson_iter_init(&iter, &document) &&
                    bson_iter_find(&iter, "precursor") &&
                    BSON_ITER_HOLDS_OID(&iter)) {
                    bson_oid_copy(bson_iter_oid(&iter), &oid_precursor);
                } else{
                    bson_oid_copy(&oid_document, &oid_precursor);
                }

                // time_of_death
                if (bson_iter_init(&iter, &document) &&
                    bson_iter_find(&iter, "death") &&
                    BSON_ITER_HOLDS_INT64(&iter)) {
                    time_of_death = bson_iter_int64(&iter);
                } else{
                    time_of_death = 0;
                }
            }

            if (mongoc_cursor_error (cursor, &error)) {
                std::cerr << "An error occurred: "<<  error.message << std::endl;
                return false;
            }
            // Clean up
            bson_destroy(query);
            mongoc_cursor_destroy(cursor);
            return true;
        }

    } else{
        std::cerr << "OID string not valid." << std::endl;
        return false;
    }
}

bool MongoObject::read_from_db() {
    return read_from_db(oid_to_string(oid_document));
}


std::string MongoObject::get_json() {
    size_t len;
    bson_t doc = get_bson();
    if(&doc){
        char* str = bson_as_json (&doc, &len);
        return std::string(str, len);
    } else{
        return "";
    }
}

bson_t MongoObject::get_bson(){

    bson_iter_t iter;

    // oid_document
    if (bson_iter_init(&iter, &document) &&
        bson_iter_find(&iter, "_id") &&
        BSON_ITER_HOLDS_OID(&iter)) {
        bson_iter_overwrite_oid(&iter, &oid_document);
    } else{
        bson_append_oid(&document, "_id", 3, &oid_document);
    }

    // oid_precursor
    if (bson_iter_init(&iter, &document) &&
        bson_iter_find(&iter, "precursor") &&
        BSON_ITER_HOLDS_OID(&iter)) {
        bson_iter_overwrite_oid(&iter, &oid_precursor);
    } else{
        bson_append_oid(&document, "precursor", 9, &oid_precursor);
    }

    // time of death
    if (bson_iter_init(&iter, &document) &&
        bson_iter_find(&iter, "death") &&
        BSON_ITER_HOLDS_INT64(&iter)) {
        bson_iter_overwrite_int64(&iter, time_of_death);
    } else{
        bson_append_int64(&document, "death", 5, time_of_death);
    }

    return document;
}

bson_t MongoObject::get_bson_excluding(...){
    bson_t src = MongoObject::get_bson();
    bson_t dst; bson_init (&dst);
    va_list va;


     // this is new in libmongoc=1.14.0
    bson_copy_to_excluding_noinit_va(
            &src, &dst,
            "",
            va
            );
    return dst;
}

std::string MongoObject::get_oid_string(){
    return oid_to_string(oid_document);
}

bson_oid_t MongoObject::get_oid() {
    return oid_document;
}

const bson_t* MongoObject::get_document(){
    return &document;
}

std::string MongoObject::create_copy() {
    bson_t document_copy;

    // update oid of copy
    bson_oid_t oid_copy;
    bson_oid_init(&oid_copy, nullptr);
    bson_copy_to(get_document(), &document_copy);
    bson_iter_t iter;
    if (bson_iter_init(&iter, &document_copy) &&
        bson_iter_find(&iter, "_id") &&
        BSON_ITER_HOLDS_OID(&iter)) {
        bson_iter_overwrite_oid(&iter, &oid_copy);
    }

    // set precursor of copy to current document
    if (bson_iter_init(&iter, &document_copy) &&
        bson_iter_find(&iter, "precursor") &&
        BSON_ITER_HOLDS_OID(&iter)) {
        bson_iter_overwrite_oid(&iter, &oid_document);
    } else{
        bson_append_oid(&document_copy, "precursor", 9, &oid_document);
    }

    size_t len;
    std::clog << "created copy: " << bson_as_json(&document_copy, &len) << std::endl;

    write_to_db(document_copy, 2);

    return oid_to_string(oid_copy);
}

bool MongoObject::is_connected_to_db() {
    return is_connected_to_db_;
}


/*
template<typename T>
T MongoObject::get_value(const char* key) {
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
 */

/*
template <typename T>
void MongoObject::set_value(const char* key, T value){
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
*/

void MongoObject::set_value(const char* key, bson_oid_t value){
    bson_iter_t iter;
    if (bson_iter_init_find(&iter, &document, key) &&
        BSON_ITER_HOLDS_OID(&iter)) {
        bson_iter_overwrite_oid(&iter, &value);
    }
}


/*
template <typename T>
std::vector<T> MongoObject::get_array(const char* key){
    bson_iter_t iter;

    // double
    if(std::is_same<T, std::vector<double>>::value){
        try {
            return buff_double_vector_.at(key);
        }
        catch (std::out_of_range &o){
            std::vector<double> v;
            if (bson_iter_init_find(&iter, &document, key) &&
                (BSON_ITER_HOLDS_ARRAY(&iter))){
                bson_iter_t iter_array;
                bson_iter_recurse (&iter, &iter_array);
                while (bson_iter_next (&iter_array) &&
                       BSON_ITER_HOLDS_DOUBLE(&iter_array)) {
                    v.push_back(bson_iter_double(&iter_array));
                }
            }
            buff_double_vector_[key] = v;
            return v;
        }
    }
    std::vector<T> v{};
    return v;
}
 */

/*
template <typename T>
void MongoObject::set_array(const char* key, T &value){
    T v{value};
    if(std::is_same<T, std::vector<double>>::value) {
        buff_double_vector_[key] = v;
    }
}
 */


bool MongoObject::string_to_oid(const std::string &oid_string, bson_oid_t *oid){
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

/*
template <typename T>
void MongoObject::create_oid_dict_in_doc(
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
*/


template <typename T>
void MongoObject::create_oid_array_in_doc(
        bson_t *doc,
        std::string target_field_name,
        const std::set<std::shared_ptr<T>> &mongo_obj_array){

    bson_t child;
    bson_append_array_begin(doc, target_field_name.c_str(), target_field_name.size(), &child);
    for(auto &v : mongo_obj_array){
        const bson_oid_t b = v->get_oid();
        bson_append_oid(&child, "", 0, &b);
    }
    bson_append_array_end(doc, &child);
}

/*
template <typename T>
bool MongoObject::create_and_connect_objects_from_oid_doc(
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
*/

template <typename T>
bool MongoObject::create_and_connect_objects_from_oid_array(
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

void MongoObject::append_string(bson_t *dst, const std::string key, const std::string content){
    bson_append_utf8(dst, key.c_str(), key.size(), content.c_str(), content.size());
}

const std::string MongoObject::get_string_by_key(bson_t *doc, const std::string key){
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
