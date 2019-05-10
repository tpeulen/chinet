//
// Created by thomas on 4/23/19.
//

#ifndef CHINET_SESSION_H
#define CHINET_SESSION_H

#include <map>
#include <memory>
#include "bson.h"

#include "MongoObject.h"
#include "Node.h"
#include "Port.h"

class Session : public MongoObject{

protected:
    bson_t get_bson() final;

public:

    std::map<std::string, std::shared_ptr<Node>> nodes;

    Session(){
        bson_append_utf8(&document, "type", 4, "session", 7);
    };

    Session(std::map<std::string, std::shared_ptr<Node>> nodes) :
    Session()
    {
        for(auto &o : nodes){
            add_node(o.first, o.second);
        }
    };

    void add_node(std::string name, std::shared_ptr<Node> object){
        nodes[name] = object;
        object->set_name(name);
        if(is_connected_to_db()){
            connect_object_to_db(object);
        }
    }

    std::map<std::string, std::shared_ptr<Node>> get_nodes(){
        return nodes;
    }

    bool write_to_db() final;
    bool read_from_db(const std::string &oid_string) final;

};


#endif //CHINET_SESSION_H
