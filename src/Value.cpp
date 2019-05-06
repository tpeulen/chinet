#include "Value.h"

/*
Value::Value() :
fixed(true)
{
    bson_oid_init(&oid, nullptr);
    bson_oid_copy(&oid, &oid_precursor);
    port = nullptr;
}

Value::Value(std::vector<double> v, bool fixed) : Value()
{
    this->content = v;
    this->fixed = fixed;
}

Value::Value(std::vector<double> v, bool fixed, Value *precursor) :
Value(v, fixed)
{
    oid_precursor = precursor->oid_precursor;
}

Value::~Value() {
}

bson_t* Value::to_bson() {
    bson_t *doc = MongoObject::to_bson();

    bson_oid_t oid_port = (port == nullptr) ? oid : port->oid;

    // create a bson for the values

    doc = BCON_NEW(
            "_id", BCON_OID(&oid),
            "fixed", BCON_BOOL(fixed),
            "port", BCON_OID(&oid_port)
            );

    bson_t child;
    bson_append_array_begin(doc, "value", -1, &child);
    for(auto v : content){
        bson_append_double(&child, "", 0, v);
    }
    bson_append_array_end(doc, &child);
    // append the value to the doc

    return doc;
}
 */

/*
bool Value::from_bson(const bson_t *doc){
    bson_iter_t iter;
    bson_iter_t child;

    if (bson_iter_init_find (&iter, doc, "oid") &&
        BSON_ITER_HOLDS_OID (&iter)) {
        oid = *bson_iter_oid(&iter);
    }

    if (bson_iter_init_find (&iter, doc, "fixed") &&
        BSON_ITER_HOLDS_BOOL (&iter)) {
        fixed = bson_iter_bool(&iter);
    }

    if (bson_iter_init_find (&iter, doc, "precursor") &&
            BSON_ITER_HOLDS_OID (&iter)) {
        oid_precursor = *bson_iter_oid(&iter);
    }

    if (bson_iter_init_find (&iter, doc, "port") &&
        BSON_ITER_HOLDS_OID (&iter)) {
        oid_port = *bson_iter_oid(&iter);
    }

    content.resize(0);
    if (bson_iter_init_find (&iter, doc, "value") &&
    BSON_ITER_HOLDS_ARRAY(&iter) &&
    bson_iter_recurse (&iter, &child)) {
        while (bson_iter_next (&child)) {
            content.push_back(bson_iter_double(&child));
        }
    }

    return true;
}

bool Value::from_json(const std::string &json_string){
    bson_error_t error;
    const bson_t *doc = bson_new_from_json(
            (uint8_t*)json_string.c_str(),
            json_string.size(),
            &error
    );
    return from_bson(doc);

}
*/

/*
std::vector<double> Value::get_value() {
    return content;
}

uint64_t Value::get_time() {
    return bson_oid_get_time_t(&oid);
}

void Value::push_back(double v) {
    content.push_back(v);
}
 */