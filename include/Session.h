//
// Created by thomas on 4/23/19.
//

#ifndef CHINET_SESSION_H
#define CHINET_SESSION_H

#include <mongoc.h>
#include <iostream>
#include <set>
#include <memory>

#include "MongoObject.h"
#include "Node.h"
#include "Port.h"
#include "Link.h"


class Session : public MongoObject{

protected:

    bool add_and_connect_object(std::shared_ptr<MongoObject> object){
        registered_objects.push_back(object);
        return connect_object_to_db(object);
    }

public:
    std::vector<std::shared_ptr<MongoObject>> registered_objects;

    std::set<std::shared_ptr<Node>> nodes;
    std::set<std::shared_ptr<Port>> ports;
    std::set<std::shared_ptr<Link>> links;

    Session(){
        bson_append_utf8(&document, "type", 4, "session", 7);
    };

    void add_node(std::shared_ptr<Node> object){
        nodes.insert(object);
        add_and_connect_object(object);
    }

    void add_port(std::shared_ptr<Port> object){
        ports.insert(object);
        add_and_connect_object(object);
    }

    void add_link(std::shared_ptr<Link> object){
        links.insert(object);
        add_and_connect_object(object);
    }

    bool write_to_db() final;
    bool read_from_db(const std::string &oid_string) final;
    bson_t get_bson() final;

};


#endif //CHINET_SESSION_H
