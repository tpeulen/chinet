#include "Port.h"


// Constructor
//--------------------------------------------------------------------

Port::Port(){
}

Port::Port(std::string json_string) :
Port(json_string, nullptr)
{
}


Port::Port(std::shared_ptr<Node> node) :
Port("", node)
{
}

Port::Port(std::string json_string, std::shared_ptr<Node> node){
    //bson_oid_init(&oid, NULL);
    this->node = node;
    from_json(json_string);
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

/*
std::string Port::get_oid(){
    char * oid_s = new char[25];
    bson_oid_to_string(&oid, oid_s);
    return std::string(oid_s);
}
 */

/*
std::string Port::get_name(){
    bson_iter_t iter;
    uint32_t len{0};
    const char * str;

    if (bson_iter_init (&iter, b) &&
        bson_iter_find (&iter, "name") &&
        BSON_ITER_HOLDS_UTF8 (&iter)) {
        str = bson_iter_utf8(&iter, &len);
        return std::string(str, len);
    }
    return "";
}
 */

std::shared_ptr<Port> Port::shared_ptr() {
    return std::shared_ptr<Port>(this);
}

bson_t* Port::get_value(){
    if(input == nullptr){
        return b;
    } else{
        return input->get_value();
    }
}

double Port::get_slot_value(const std::string &slot_key){
    bson_iter_t iter;
    bson_iter_t baz;

    //std::string search_values = "slots." + slot_key + ".value";
    std::string search_values = slot_key + ".value";
    if (bson_iter_init (&iter, get_value()) &&
        bson_iter_find_descendant (&iter, search_values.c_str(), &baz) &&
        BSON_ITER_HOLDS_DOUBLE (&baz)) {
        return bson_iter_double(&baz);
    }
    return NAN;
}

std::vector<double> Port::get_slot_values(){
    std::vector<double> values;
    bson_iter_t i0;
    //bson_iter_t i1;
    bson_iter_t i2;
    bson_iter_t i3;

    //bson_iter_init(&i1, get_value());
    //bson_iter_find(&i1, "slots");
    //bson_iter_recurse (&i1, &i2);
    bson_iter_init(&i2, get_value());

    while (bson_iter_next (&i2)) {
        //std::string search_values = "slots." + std::string(bson_iter_key(&i2)) + ".value";
        std::string search_values = std::string(bson_iter_key(&i2)) + ".value";
        bson_iter_init(&i0, get_value());
        if (bson_iter_init (&i3, get_value()) &&
            bson_iter_find_descendant (&i0, search_values.c_str(), &i3) &&
            BSON_ITER_HOLDS_DOUBLE (&i3)) {
            values.emplace_back(bson_iter_as_double (&i3));
        }
    }
    return values;
}

std::vector<std::string> Port::get_slot_keys(){
    std::vector<std::string> names;
    //bson_iter_t iter;
    bson_iter_t iter_s;

    //bson_iter_init(&iter, get_value());
    //bson_iter_find(&iter, "slots");
    //bson_iter_recurse (&iter, &iter_s);
    bson_iter_init(&iter_s, get_value());

    while (bson_iter_next (&iter_s)) {
        names.emplace_back(bson_iter_key (&iter_s));
    }
    return names;
}

std::shared_ptr<Port> Port::get_input(){
    return input;
}

// Setter
//--------------------------------------------------------------------

/*
void Port::set_oid(std::string v){
    bson_oid_init_from_string (&oid, v.c_str());
}
 */

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

void Port::set_input(std::shared_ptr<Port> v){
    this->input = v;
}


// Methods
// --------------------------------------------------------------------

void Port::from_json(const std::string &json_string){
    bson_error_t error;
    b = bson_new_from_json(
            (uint8_t*)json_string.c_str(),
            json_string.size(),
            &error
            );

    if (!b) {
        printf ("Error: %s\n", error.message);
    } /*else{
        bson_oid_init (&oid, NULL);
        BSON_APPEND_OID (b, "_id", &oid);
    }*/
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
