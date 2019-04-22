#include "Port.h"


// Constructor
//--------------------------------------------------------------------

Port::Port(){
    bson_oid_init(&oid, nullptr);

    document = BCON_NEW(
            "_id", BCON_OID(&oid),
            "predecessor", BCON_OID(&oid),
            "birth", BCON_INT64(Functions::get_time()),
            "death", BCON_INT64(0)
    );
}

Port::Port(bson_oid_t oid){
    this->oid = oid;

    document = BCON_NEW(
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
    bson_destroy(document);
}

// Operator
//--------------------------------------------------------------------
std::shared_ptr<Port> Port::operator+(std::shared_ptr<Port> v){
    bson_oid_t new_port_oid;
    bson_oid_init(&new_port_oid, nullptr);
    auto r = std::make_shared<Port>(new_port_oid);
    bson_init(r->document);
    r->document = BCON_NEW(
            "_id", BCON_OID(&new_port_oid),
            "predecessor", BCON_OID(&new_port_oid),
            "birth", BCON_INT64(Functions::get_time()),
            "death", BCON_INT64(0)
    );

    std::vector<std::string> skip = {"_id", "predecessor", "birth", "death"};
    Functions::add_documents(this->document, r->document, skip);
    Functions::add_documents(v->document, r->document, skip);

    return r;
}

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

    if (bson_iter_init (&iter, document) &&
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
        return document;
    } else{
        return input->get_value();
    }
}

std::vector<double> Port::get_slot_value(const std::string &slot_key){
    bson_iter_t iter;
    bson_iter_t baz;
    std::vector<double> v;

    //std::string search_values = "slots." + slot_key + ".value";
    std::string search_values = slot_key + ".value";
    bson_t* doc = get_value();
    if(doc != nullptr){
        if (bson_iter_init (&iter, doc) && bson_iter_find_descendant (&iter, search_values.c_str(), &baz)) {
            if(BSON_ITER_HOLDS_DOUBLE (&baz)){
                v.push_back(bson_iter_double(&baz));
            }
            if(BSON_ITER_HOLDS_ARRAY(&baz)){
                bson_iter_t iter_array;
                bson_iter_recurse (&iter, &iter_array);
                while (bson_iter_next (&iter_array)) {
                    v.push_back(bson_iter_double(&iter_array));
                }
            }
        }
    } else{
        std::cerr << "Port document not initialized.";
        v.push_back(NAN);
    }
    return v;
}

std::map<std::string, std::vector<double>> Port::get_slot_values(){

    std::map<std::string, std::vector<double>> r;
    bson_iter_t i0;
    bson_iter_t i1;
    bson_iter_t ik;

    bson_t* doc = get_value();
    if(doc != nullptr){
        bson_iter_init(&i0, doc);
        bson_iter_init(&ik, doc);
        while (bson_iter_next (&ik)) {
            std::string key = std::string(bson_iter_key(&ik));
            std::string search_values = key + ".value";

            bson_iter_init(&i0, doc);
            if (bson_iter_init (&i0, doc) &&
                bson_iter_find_descendant (&i0, search_values.c_str(), &i1) &&
                (BSON_ITER_HOLDS_DOUBLE (&i1) || BSON_ITER_HOLDS_ARRAY(&i1))
                ){

                std::vector<double> slot_values;
                if(BSON_ITER_HOLDS_ARRAY(&i1)){
                    bson_iter_t iter_array;
                    bson_iter_recurse (&i1, &iter_array);
                    while (bson_iter_next (&iter_array)) {
                        slot_values.push_back(bson_iter_double(&iter_array));
                    }
                }
                if(BSON_ITER_HOLDS_DOUBLE (&i1)){
                    slot_values.push_back(bson_iter_as_double (&i1));
                }
                r[key] = slot_values;
            }
        }
    } else{
        std::cerr << "Port document not initialized.";
    }
    return r;
}

std::vector<std::string> Port::get_slot_keys(){
    std::vector<std::string> names;
    bson_iter_t i0;
    bson_iter_t i1;
    bson_iter_t ik;

    bson_t* doc = get_value();
    if(doc != nullptr){
        bson_iter_init(&i0, doc);
        bson_iter_init(&ik, doc);
        while (bson_iter_next (&ik)) {
            std::string search_values = std::string(bson_iter_key(&ik)) + ".value";
            bson_iter_init(&i0, doc);
            if (bson_iter_init (&i0, doc) &&
                bson_iter_find_descendant (&i0, search_values.c_str(), &i1) &&
                    (BSON_ITER_HOLDS_DOUBLE (&i1) || BSON_ITER_HOLDS_ARRAY(&i1))
                ){
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
    bson_oid_init(&oid, nullptr);
    bson_iter_t iter;

    bson_t *doc = document;
    if(doc != nullptr) {
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

    bson_t *doc = document;
    if(doc != nullptr) {
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

bool Port::set_predecessor(bson_oid_t v){
    bson_iter_t iter;
    bson_t *doc = document;
    if(doc != nullptr) {
        char oidstr[25];
        if (bson_iter_init(&iter, doc) && bson_iter_find(&iter, "predecessor") && BSON_ITER_HOLDS_OID (&iter)) {
            bson_oid_to_string(bson_iter_oid(&iter), oidstr);
            std::clog << "set predecessor: " << oidstr << std::endl; 
            bson_iter_overwrite_oid(&iter, &v);
            return true;
        }
    } else{
        std::cerr << "Error: set_predecessor, port document not initialized.";
    }
    return false;
}

bool Port::set_death_time(uint64_t v){
    bson_iter_t iter;
    bson_t *doc = document;
    if(doc != nullptr) {
        char oidstr[25];
        if (bson_iter_init(&iter, doc) && bson_iter_find(&iter, "death") && BSON_ITER_HOLDS_INT64(&iter)) {
            bson_oid_to_string(bson_iter_oid(&iter), oidstr);
            bson_iter_overwrite_int64(&iter, v);
            std::clog << "set oid " << get_oid() << "death: " << v << std::endl;
            return true;
        }
    } else{
        std::cerr << "Error: set_death_time, port document not initialized.";
    }
    return false;
}

bool Port::set_birth_time(uint64_t v){
    bson_iter_t iter;
    bson_t *doc = document;
    if(doc != nullptr) {
        char oidstr[25];
        if (bson_iter_init(&iter, doc) && bson_iter_find(&iter, "birth") && BSON_ITER_HOLDS_INT64(&iter)) {
            bson_oid_to_string(bson_iter_oid(&iter), oidstr);
            bson_iter_overwrite_int64(&iter, v);
            std::clog << "set oid " << get_oid() << "birth: " << v << std::endl;
            return true;
        }
    } else{
        std::cerr << "Error: set_birth_time, port document not initialized.";
    }
    return false;
}

// Methods
// --------------------------------------------------------------------
void Port::from_json(const std::string &json_string){
    std::clog << "Port.from_json:";
    bson_error_t error;
    document = bson_new_from_json((uint8_t*)json_string.c_str(), json_string.size(), &error);
    if (!document) {
        std::cerr << "Error: " << error.message;
    } else{
        // find the oid
        bson_iter_t iter;
        char oidstr[25];
        if (bson_iter_init (&iter, document) &&
            bson_iter_find (&iter, "_id") &&
            BSON_ITER_HOLDS_OID (&iter)) {
            oid = *bson_iter_oid(&iter);
            bson_oid_to_string (&oid, oidstr);
            std::clog << "OID:" << oidstr;
        } else {
            std::clog << "no OID:";
            bson_oid_init (&oid, nullptr);
            BSON_APPEND_OID (document, "_id", &oid);
            std::clog << "Generated OID: " << get_oid();
        }
    }
    std::clog << std::endl;
}

std::string Port::to_json(){
    if(document == nullptr){
        return "";
    } else{
        size_t len;
        char* str = bson_as_json (document, &len);
        return std::string(str, len);
    }
}

/*
bool Port::add_port_to_collection(mongoc_collection_t * port_collection){
    return mongoc_collection_insert_one(port_collection,
            document,
            nullptr, nullptr, nullptr
            );
}
*/
