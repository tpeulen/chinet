#include "Port.h"


// Constructor
//--------------------------------------------------------------------

Port::Port(){
    bson_oid_init(&oid, NULL);

    b = BCON_NEW(
            "_id", BCON_OID(&oid),
            "predecessor", BCON_OID(&oid),
            "birth", BCON_INT64(Functions::get_time()),
            "death", BCON_INT64(0)
    );
}

Port::Port(bson_oid_t oid){
    this->oid = oid;

    b = BCON_NEW(
            "_id", BCON_OID(&oid),
            "predecessor", BCON_OID(&oid),
            "birth", BCON_INT64(Functions::get_time()),
            "death", BCON_INT64(0)
    );

}

Port::Port(std::string json_string) :
Port(){
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

std::string Port::get_oid(){
    char * oid_s = new char[25];
    bson_oid_to_string(&oid, oid_s);
    return std::string(oid_s);
}

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
    bson_t* doc = get_value();
    if(doc != NULL){
        if (bson_iter_init (&iter, doc) &&
            bson_iter_find_descendant (&iter, search_values.c_str(), &baz) &&
            BSON_ITER_HOLDS_DOUBLE (&baz)) {
            return bson_iter_double(&baz);
        }
    } else{
        std::cerr << "Port document not initialized.";
    }
    return NAN;
}

std::vector<double> Port::get_slot_values(){
    std::vector<double> values;
    bson_iter_t i0;
    bson_iter_t i2;
    bson_iter_t i3;

    bson_iter_init(&i2, get_value());

    bson_t* doc = get_value();
    if(doc != NULL){
        while (bson_iter_next (&i2)) {
            std::string search_values = std::string(bson_iter_key(&i2)) + ".value";
            bson_iter_init(&i0, doc);
            if (bson_iter_init (&i3, doc) &&
                bson_iter_find_descendant (&i0, search_values.c_str(), &i3) &&
                BSON_ITER_HOLDS_DOUBLE (&i3)) {
                values.emplace_back(bson_iter_as_double (&i3));
            }
        }
    } else{
        std::cerr << "Port document not initialized.";
    }
    return values;
}

std::vector<std::string> Port::get_slot_keys(){
    std::vector<std::string> names;
    bson_iter_t i0;
    bson_iter_t i1;
    bson_iter_t ik;

    bson_t* doc = get_value();
    if(doc != NULL){
        bson_iter_init(&i0, doc);
        bson_iter_init(&ik, doc);
        while (bson_iter_next (&ik)) {
            std::string search_values = std::string(bson_iter_key(&ik)) + ".value";
            bson_iter_init(&i0, doc);
            if (bson_iter_init (&i0, doc) &&
                bson_iter_find_descendant (&i0, search_values.c_str(), &i1) &&
                BSON_ITER_HOLDS_DOUBLE (&i1)){
                names.emplace_back(bson_iter_key (&ik));
            }
        }
    } else{
        std::cerr << "Port document not initialized.";
    }
    return names;
}

std::shared_ptr<Port> Port::get_input(){
    return input;
}

// Setter
//--------------------------------------------------------------------

void Port::set_oid(std::string v){
    bson_oid_init_from_string (&oid, v.c_str());
}

void Port::new_oid(){
    bson_oid_init(&oid, NULL);
    bson_iter_t iter;

    bson_t *doc = b;
    if(doc != NULL) {
        if (bson_iter_init(&iter, doc) &&
            bson_iter_find(&iter, "_id") &&
            BSON_ITER_HOLDS_OID (&iter)) {
            bson_iter_overwrite_oid(&iter, &oid);
            std::cout << "new oid: " << get_oid() << std::endl;
        }
    } else{
        std::cerr << "Port document not initialized.";
    }
}

bool Port::set_slot_value(std::string slot_key, double value){
    bson_iter_t iter;
    bson_iter_t baz;

    bson_t *doc = b;
    if(doc != NULL) {
        std::string search_values = slot_key + ".value";
        if (bson_iter_init(&iter, doc) &&
            bson_iter_find_descendant(&iter, search_values.c_str(), &baz) &&
            BSON_ITER_HOLDS_DOUBLE (&baz)) {
            bson_iter_overwrite_double(&baz, value);
            return true;
        }
    }else{
        std::cerr << "Error: set_slot_value, port document not initialized.";
    }
    return false;
}

void Port::set_input(std::shared_ptr<Port> v){
    this->input = v;
}

void Port::set_predecessor(bson_oid_t v){
    bson_iter_t iter;
    bson_t *doc = b;
    if(doc != NULL) {
        char oidstr[25];
        if (bson_iter_init(&iter, doc) && bson_iter_find(&iter, "predecessor") && BSON_ITER_HOLDS_OID (&iter)) {
            bson_oid_to_string(bson_iter_oid(&iter), oidstr);
            printf("predecessor: %s\n", oidstr);
            bson_iter_overwrite_oid(&iter, &v);
        }
    } else{
        std::cerr << "Error: set_predecessor, port document not initialized.";
    }
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
    } else{
        // find the oid
        bson_iter_t iter;
        char oidstr[25];
        if (bson_iter_init (&iter, b) &&
            bson_iter_find (&iter, "_id") &&
            BSON_ITER_HOLDS_OID (&iter)) {
            oid = *bson_iter_oid(&iter);
            bson_oid_to_string (&oid, oidstr);
            printf ("%s\n", oidstr);
        } else {
            std::cout << "Document is missing _id." << std::endl;
            bson_oid_init (&oid, NULL);
            BSON_APPEND_OID (b, "_id", &oid);
            std::cout << "Generated id: " <<
            get_oid() <<
            std::endl;
        }
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

bool Port::add_port_to_collection(mongoc_collection_t * port_collection){
    return mongoc_collection_insert_one(port_collection,
            b,
            nullptr, nullptr, nullptr
            );
}

