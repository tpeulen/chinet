#include "Port.h"



bool Port::is_fixed(){
    return MongoObject::get_singleton<bool>("fixed");
}

bool Port::is_linked(){
    return (link_ != nullptr);
}

void Port::link(std::shared_ptr<Port> &v){
    if(v != nullptr){
        MongoObject::set_singleton("link", v->get_bson_oid());
        link_ = v;
    }
}

void Port::unlink(){
    MongoObject::set_singleton("link", get_bson_oid());
    link_ = nullptr;
}

void Port::set_fixed(bool fixed){
    MongoObject::set_singleton("fixed", fixed);
}

bson_t Port::get_bson() {
    bson_t dst = MongoObject::get_bson_excluding("value", NULL);
    MongoObject::append_number_array(&dst, "value", buff_double_vector_);
    return dst;
}


// Constructor
//--------------------------------------------------------------------

// Destructor
//--------------------------------------------------------------------


// Operator
//--------------------------------------------------------------------

//std::shared_ptr<Port> Port::operator+(std::shared_ptr<Port> v){
//    bson_oid_t new_port_oid;
//    bson_oid_init(&new_port_oid, nullptr);
//    auto r = std::make_shared<Port>(new_port_oid);
//    bson_init(r->document);
//    r->document = BCON_NEW(
//            "_id", BCON_OID(&new_port_oid),
//            "predecessor", BCON_OID(&new_port_oid),
//            "birth", BCON_INT64(Functions::get_time()),
//            "death", BCON_INT64(0)
//    );
//
//    Functions::add_documents(this->document, r->document, skip);
//    Functions::add_documents(v->document, r->document, skip);
//
//    return r;
//}

/*
std::vector<double> Port::operator[](std::string key){
    return value(key);
}
 */

// Getter
//--------------------------------------------------------------------

/*
std::vector<std::string> Port::keys(){
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
 */


// Setter
//--------------------------------------------------------------------


//bool Port::set_slot_value(std::string slot_key, std::vector<double> values){
//    bson_iter_t iter;
//    bson_iter_t baz;
//
//    bson_t *doc = document;
//    if(doc != nullptr) {
//        std::string search_values = slot_key + ".value";
//        if (bson_iter_init(&iter, doc) &&
//            bson_iter_find_descendant(&iter, search_values.c_str(), &baz)
//            ) {
//            if(BSON_ITER_HOLDS_DOUBLE (&baz)){
//
//            } else{
//                if(BSON_ITER_HOLDS_ARRAY(&baz)){
//                    /*
//                    bson_count_keys(bson_iter_);
//                    int narray;
//                    const uint8_t **array;
//                    bson_iter_array(&baz, &narray, array);
//                     */
//                }
//            }
//            /*
//            if(bson_count_keys(&baz))
//            for(auto &v : values){
//                bson_a
//            }
//            bson_iter_overwrite_double(&baz, value);
//            return true;*/
//        }
//    }else{
//        std::cerr << "Error: set_slot_value, port document not initialized.";
//    }
//    return false;
//}


/*
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
 */


// Methods
// --------------------------------------------------------------------
/*
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
*/



/*
size_t Port::size(){
    bson_iter_t iter;
    size_t s = 0;
    if (bson_iter_init (&iter, document)) {
        while (bson_iter_next (&iter)) {
            if(!Functions::bson_iter_skip(&iter, &skip)){
                s++;
            }
        }
    }
    return s;
}
 */

