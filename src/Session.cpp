//
// Created by thomas on 4/23/19.
//

#include "Session.h"

bool Session::read_from_db(const std::string &oid_string){
    bool return_value = true;
    return_value &= MongoObject::read_from_db(oid_string);
    return_value &= create_and_connect_objects_from_oid_array(&document, "nodes", &nodes);
    return return_value;
}

bool Session::write_to_db(){

    bool re = MongoObject::write_to_db();

    for(auto &o : nodes){
        if(!o.second->is_connected_to_db()){
            re &= connect_object_to_db(o.second);
        }
        o.second->write_to_db();
    }

    return re;
}

bson_t Session::get_bson(){
    bson_t doc2 = MongoObject::get_bson();

    bson_t doc; bson_init (&doc);
    bson_copy_to_excluding_noinit(
            &doc2, &doc,
            "nodes",
            NULL
            );

    create_oid_array_in_doc(&doc, "nodes", nodes);

    return doc;
}



