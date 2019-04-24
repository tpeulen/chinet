#include "Value.h"


Value::Value() :
fixed(true),
precursor(nullptr)
{
    bson_oid_init(&oid, nullptr);
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
    this->precursor = precursor;
}

Value::~Value() {
}


bson_t* Value::to_bson() {
    bson_t *doc;

    bson_oid_t oid_port = (port == nullptr) ? oid : port->oid;
    bson_oid_t oid_precursor = (port == nullptr) ? oid : precursor->oid;

    // create a bson for the values

    doc = BCON_NEW(
            "_id", BCON_OID(&oid),
            "fixed", BCON_BOOL(fixed),
            "precursor", BCON_OID(&oid_precursor),
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

std::string Value::to_json() {
    size_t len;
    char* str = bson_as_json (to_bson(), &len);
    return std::string(str, len);
}

std::vector<double> Value::get_value() {
    return content;
}

uint64_t Value::get_time() {
    return bson_oid_get_time_t(&oid);
}

bool Value::push_back(double v) {
    content.push_back(v);
}