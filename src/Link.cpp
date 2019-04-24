//
// Created by thomas on 4/22/19.
//

#include "Link.h"


Link::Link() {
    bson_oid_init(&oid, nullptr);
}

Link::Link(Port *target, std::string key){
    this->target = target;
    this->key = key;
}

Value* Link::get_target_value(){
    return target->get_v(key);
}

std::string Link::to_json() {
    size_t len;
    char* str = bson_as_json (to_bson(), &len);
    return std::string(str, len);
}

bson_t* Link::to_bson() {
    bson_t *doc;

    // create a bson for the values
    if(target != nullptr){
        doc = BCON_NEW(
                "_id", BCON_OID(&oid),
                "port", BCON_OID(&target->oid),
                "key", BCON_UTF8(key.c_str())
        );
    } else{
        doc = BCON_NEW(
                "_id", BCON_OID(&oid)
        );
    }

    return doc;
}


bool Link::set_target(Port *target, std::string key){
    this->target = target;
    this->key = key;
}
