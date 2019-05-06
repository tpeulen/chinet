//
// Created by thomas on 4/22/19.
//

#include "Link.h"


Link::Link(){
    bson_append_utf8(&document, "type", 4, "link", 4);
    bson_append_oid(&document, "target", 6, &oid_document);
}

double Link::get_target_value_double(){
    if(target!= nullptr){
        return target->get_value_double();
    } else{
        std::cerr << "Error - class Link get_value_double: target is nullptr" << std::endl;
        return NAN;
    }
}
int Link::get_target_value_int(){
    if(target!= nullptr){
        return target->get_value_int();
    } else{
        std::cerr << "Error - class Link get_value_int: target is nullptr" << std::endl;
        return 0;
    }
}

void Link::set_target_value(double v){
    if(target!= nullptr){
        target->set_value(v);
    } else{
        std::cerr << "Error - class Link set_value: target is nullptr" << std::endl;
    }
}

void Link::set_target(std::shared_ptr<Port> v) {
    overwrite_oid_in_field(v->get_oid(), "target");
    target = v;
}



/*
Link::Link(std::shared_ptr<Port> target, std::string key) :
Link()
{
    this->target = target;
    this->key = key;
}

*/