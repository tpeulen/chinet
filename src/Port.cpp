#include "Port.h"

int Port::sNextId = 0;


// Constructor
//--------------------------------------------------------------------
Port::Port(
        std::shared_ptr<Node> &node
        ) :
        Port("", node)
{
}

Port::Port(
        std::string name,
        std::shared_ptr<Node> &node
        ) :
        Port(name)
{
    this->node = node;
}


Port::Port(std::string name) : Port(){
    this->name = name;
}

// Destructor
//--------------------------------------------------------------------
Port::~Port() {
    bson_destroy(b);
}

// Operator
//--------------------------------------------------------------------


// Getter
//--------------------------------------------------------------------

std::string Port::get_oid(){
    char * oid_s = new char[25];
    bson_oid_to_string(&oid, oid_s);
    return std::string(oid_s);
}

std::string Port::get_name(){
    return name;
}

std::shared_ptr<Port> Port::shared_ptr() {
    return std::shared_ptr<Port>(this);
}

// Setter
//--------------------------------------------------------------------
void Port::set_name(std::string &v){
    name = v;
}

void Port::set_oid(std::string v){
    bson_oid_init_from_string (&oid, v.c_str());
}

void Port::set_slot_value(std::string slot_key, double value){
    bson_iter_t iter;
    bson_iter_t baz;

    std::string search_values = slot_key + ".value";
    if (bson_iter_init (&iter, b) &&
        bson_iter_find_descendant (&iter, search_values.c_str(), &baz) &&
        BSON_ITER_HOLDS_DOUBLE (&baz)) {
        bson_iter_overwrite_double(&baz, value);
    }
}


// Methods
// --------------------------------------------------------------------
double Port::get_slot_value(const std::string &slot_key){
    bson_iter_t iter;
    bson_iter_t baz;

    std::string search_values = slot_key + ".value";
    if (bson_iter_init (&iter, b) &&
        bson_iter_find_descendant (&iter, search_values.c_str(), &baz) &&
        BSON_ITER_HOLDS_DOUBLE (&baz)) {
        return bson_iter_double(&baz);
    }
    return NAN;
}

std::vector<std::string> Port::get_slot_names(){
    std::vector<std::string> names;
    bson_iter_t iter;
    if (bson_iter_init (&iter, b)) {
        while (bson_iter_next (&iter)) {
            names.push_back(bson_iter_key (&iter));
        }
    }
    return names;
}

void Port::from_json(const std::string &json_string){
    bson_error_t error;
    b = bson_new_from_json(
            (uint8_t*)json_string.c_str(),
            json_string.size(),
            &error
            );

    if (!b) {
        printf ("Error: %s\n", error.message);
    } else{
        bson_oid_init (&oid, NULL);
        BSON_APPEND_OID (b, "_id", &oid);
    }
}

std::string Port::to_json(){
    if(b == nullptr){
        return "";
    } else{
        size_t len;
        char* str = bson_as_json (b, &len);
        return std::string(str, len);
    }
}
