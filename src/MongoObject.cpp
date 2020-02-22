#include "MongoObject.h"


MongoObject::MongoObject() :
        uri_string(""),
        db_string(""),
        app_string(""),
        collection_string(""),
        time_of_death(0),
        object_name(""),
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
    set_document(doc);
}

MongoObject::MongoObject(std::string name) :
        MongoObject()
{
    set_name(name);
}

MongoObject::~MongoObject()
{
    time_of_death = Functions::get_time();
    if (is_connected_to_db()) {
#if DEBUG
        std::clog << "Time of death: " << time_of_death << std::endl;
#endif
        write_to_db();
        disconnect_from_db();
    }
}

bool MongoObject::connect_to_db(
        const std::string &uri_string,
        const std::string &db_string,
        const std::string &app_string,
        const std::string &collection_string
)
{
    this->uri_string = uri_string;
    this->db_string = db_string;
    this->app_string = app_string;
    this->collection_string = collection_string;

    mongoc_init();

    // Database
    //----------------------------------------------------------------
#if DEBUG
    std::clog << "connecting to:" << uri_string.c_str() << std::endl;
#endif

    uri = mongoc_uri_new_with_error(uri_string.c_str(), &error);
    if (!uri) {
#if DEBUG
        std::cerr << "failed to parse URI:" << uri_string.c_str() << std::endl;
        std::cerr << "error message:       " << error.message << std::endl;
#endif
        return false;
    } else {
        /*
        * Create a new client instance
        */
        client = mongoc_client_new_from_uri(uri);
        /*
        * Register the application name so we can track it in the profile logs
        * on the server. This can also be done from the URI (see other examples).
        */
        mongoc_client_set_appname(client, app_string.c_str());
        /*
        * Get a handle on the collection
        */
        collection = mongoc_client_get_collection(client, db_string.c_str(), collection_string.c_str());
        is_connected_to_db_ = true;
        return true;
    }
}

void MongoObject::disconnect_from_db()
{
    if (is_connected_to_db()) {
        // destroy cursor, collection, session before the client they came from
        if (collection) {
            mongoc_collection_destroy(collection);
        }
        if (uri) {
            mongoc_uri_destroy(uri);
        }
        if (client) {
            mongoc_client_destroy(client);
        }
        mongoc_cleanup();
    }
    is_connected_to_db_ = false;
}

bool MongoObject::write_to_db(
        const bson_t &doc,
        int write_option
    )
{
#ifdef DEBUG
    std::clog << "WRITING MONGOOBJECT TO DB" << std::endl;
#endif
    bool return_value = false;
#ifdef DEBUG
    std::clog << "-- Connected to DB: " << is_connected_to_db() << std::endl;
    std::clog << "-- Write option: " << write_option << std::endl;
#endif
    if (is_connected_to_db()) {
        return_value = true;

        bson_t *query = nullptr;
        bson_t *update = nullptr;
        bson_t reply;

        query = BCON_NEW ("_id", BCON_OID(&oid_document));

        bson_t *opts = BCON_NEW(
                "upsert", BCON_BOOL(true)
        );

        switch (write_option) {
            case 1:
#ifdef DEBUG
                std::clog << "-- Replacing object in the DB." << std::endl;
#endif
                // option 1 - write as a replacement
                if (
                    !mongoc_collection_replace_one(
                            collection,
                            query, &doc,
                            nullptr, &reply, &error
                    )
                ) {
#ifdef DEBUG
                    std::cerr << error.message;
#endif
                    return_value &= false;
                }
                break;
            case 2:
#ifdef DEBUG
                std::clog << "-- Inserting as a new object in DB." << std::endl;
#endif
                // option 2 - insert as a new document
                if (
                    !mongoc_collection_insert_one(
                        collection, &doc,
                        nullptr,
                        &reply, &error
                    )
                ) {
#ifdef DEBUG
                    std::cerr << error.message;
#endif
                    return_value &= false;
                }
                break;
            default:
#ifdef DEBUG
                std::clog << "-- Updating existing object in DB." << std::endl;
#endif
                // option 0 - write as a update
                update = BCON_NEW ("$set", BCON_DOCUMENT(&doc));
                if (!mongoc_collection_find_and_modify(
                        collection,
                        query,
                        nullptr,
                        update,
                        nullptr,
                        false,
                        true, // update by upsert: if the oid is not in the DB create a new document
                        false,
                        &reply, &error)
                        ) {
#if DEBUG
                    std::cerr << error.message;
#endif
                    return_value &= false;
                }
                break;
        }
        // destroy
        bson_destroy(update);
        bson_destroy(query);
        bson_destroy(&reply);
    } else {
        std::cerr << "ERROR: Not connected to DB - cannot write!" << std::endl;
    }
    return return_value;
}

bool MongoObject::write_to_db()
{
    return write_to_db(
            get_bson(), 0
    );
}

bool MongoObject::read_from_db(const std::string &oid_string)
{
    bson_oid_t oid;
    if (string_to_oid(oid_string, &oid)) {

        if (!is_connected_to_db()) {
#if DEBUG
            std::cerr << "Not connected to a DB." << std::endl;
#endif
            return false;
        } else {
            // find the oid in the collection
            bson_t *query = nullptr;
            query = BCON_NEW ("_id", BCON_OID(&oid));

            size_t len;
#if DEBUG
            std::clog << "reading: " << bson_as_json(query, &len) << std::endl;
#endif

            mongoc_cursor_t *cursor; // cursor pointing to the new document
            cursor = mongoc_collection_find_with_opts(
                    collection,
                    query,
                    nullptr, // the opts
                    nullptr  // the read_prefs
            );

            const bson_t *doc;
            while (mongoc_cursor_next(cursor, &doc)) {
#if DEBUG
                std::clog << "read content: " << bson_as_json(doc, &len) << std::endl;
#endif

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
                } else {
                    bson_oid_copy(&oid_document, &oid_precursor);
                }

                // time_of_death
                if (bson_iter_init(&iter, &document) &&
                    bson_iter_find(&iter, "death") &&
                    BSON_ITER_HOLDS_INT64(&iter)) {
                    time_of_death = bson_iter_int64(&iter);
                } else {
                    time_of_death = 0;
                }
            }

            if (mongoc_cursor_error(cursor, &error)) {
#if DEBUG
                std::cerr << "An error occurred: " << error.message << std::endl;
#endif
                return false;
            }
            // Clean up
            bson_destroy(query);
            mongoc_cursor_destroy(cursor);
            return true;
        }

    } else {
#if DEBUG
        std::cerr << "OID string not valid." << std::endl;
#endif
        return false;
    }
}

bool MongoObject::read_from_db()
{
    return read_from_db(oid_to_string(oid_document));
}


std::string MongoObject::get_json()
{
    size_t len;
    bson_t doc = get_bson();
    char *str = bson_as_json(&doc, &len);
    return std::string(str, len);
}

std::string MongoObject::get_json_template()
{
    size_t len;
    bson_t doc = get_bson();//get_bson_excluding("death", "birth", "precursor", NULL);
    char *str = bson_as_json(&doc, &len);
    return std::string(str, len);
}

bson_t MongoObject::get_bson()
{

    bson_iter_t iter;
    bson_t doc;
    bson_init(&doc);
    bson_copy_to(&document, &doc);

    // oid_document
    if (bson_iter_init(&iter, &doc) &&
        bson_iter_find(&iter, "_id") &&
        BSON_ITER_HOLDS_OID(&iter)) {
        bson_iter_overwrite_oid(&iter, &oid_document);
    } else {
        bson_append_oid(&doc, "_id", 3, &oid_document);
    }

    // oid_precursor
    if (bson_iter_init(&iter, &doc) &&
        bson_iter_find(&iter, "precursor") &&
        BSON_ITER_HOLDS_OID(&iter)) {
        bson_iter_overwrite_oid(&iter, &oid_precursor);
    } else {
        bson_append_oid(&doc, "precursor", 9, &oid_precursor);
    }

    // time of death
    if (bson_iter_init(&iter, &doc) &&
        bson_iter_find(&iter, "death") &&
        BSON_ITER_HOLDS_INT64(&iter)) {
        bson_iter_overwrite_int64(&iter, time_of_death);
    } else {
        bson_append_int64(&doc, "death", 5, time_of_death);
    }

    // name
    append_string(
            &doc, "name",
            object_name.c_str(), object_name.size()
            );

    return doc;
}

bson_t MongoObject::get_bson_excluding(const char *first, ...)
{
    bson_t src = MongoObject::get_bson();
    bson_t dst;
    bson_init(&dst);
    va_list va;
    va_start(va, first);
    bson_copy_to_excluding_noinit_va(&src, &dst, "", va);
    va_end(va);
    return dst;
}

std::string MongoObject::get_oid()
{
    return oid_to_string(oid_document);
}

bson_oid_t MongoObject::get_bson_oid()
{
    return oid_document;
}

const bson_t *MongoObject::get_document()
{
    return &document;
}

std::string MongoObject::create_copy()
{
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
    } else {
        bson_append_oid(&document_copy, "precursor", 9, &oid_document);
    }

    size_t len;
#if DEBUG
    std::clog << "created copy: " << bson_as_json(&document_copy, &len) << std::endl;
#endif

    write_to_db(document_copy, 2);

    return oid_to_string(oid_copy);
}

bool MongoObject::is_connected_to_db()
{
    return is_connected_to_db_;
}

void MongoObject::set_oid(const char *key, bson_oid_t value)
{
    bson_iter_t iter;
    if (bson_iter_init_find(&iter, &document, key) &&
        BSON_ITER_HOLDS_OID(&iter)) {
        bson_iter_overwrite_oid(&iter, &value);
    }
}

bool MongoObject::string_to_oid(const std::string &oid_string, bson_oid_t *oid)
{
    if (bson_oid_is_valid(oid_string.c_str(), oid_string.size())) {
        // convert the oid string to an oid
        bson_oid_init_from_string(oid, oid_string.c_str());
        return true;
    } else {
        bson_oid_init(oid, nullptr);
#if DEBUG
        std::cerr << "OID string not valid." << std::endl;
#endif
        return false;
    }
}

void MongoObject::append_string(
        bson_t *dst,
        const std::string key,
        const std::string content,
        size_t size
        )
{
    if (size != 0) {
        bson_append_utf8(
                dst,
                key.c_str(), key.size(),
                content.c_str(), size
                );
    } else {
        bson_append_utf8(
                dst,
                key.c_str(), key.size(),
                content.c_str(), content.size()
                );
    }
}

const std::string MongoObject::get_string_by_key(bson_t *doc, const std::string key)
{
    bson_iter_t iter;

    if (bson_iter_init(&iter, doc) &&
        bson_iter_find(&iter, key.c_str()) &&
        BSON_ITER_HOLDS_UTF8(&iter)) {
        const char *str;
        uint32_t len;
        str = bson_iter_utf8(&iter, &len);
        return std::string(str, len);
    }
#if DEBUG
    std::cerr << "Error: the key does not contain an string" << std::endl;
#endif

    return "";
}

bool MongoObject::read_json(std::string json_string)
{
    bson_t b;
    bson_error_t error;
    if (!bson_init_from_json(&b, json_string.c_str(), json_string.size(), &error)) {
#if DEBUG
        std::cerr << "Error reading JSON: " << error.message << std::endl;
#endif
        return false;
    } else {
        bson_reinit(&document);
        bson_copy_to(&b, &document);
        return true;
    }
}

std::shared_ptr<MongoObject> MongoObject::operator[](std::string key)
{
    auto mo = std::make_shared<MongoObject>();
    mo->read_json(
            get_json(key.c_str())
    );
    return mo;
}

void MongoObject::show(std::ostream &out) const
{
    //out << get_json().c_str() << std::endl;
}

