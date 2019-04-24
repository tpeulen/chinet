//
// Created by thomas on 4/22/19.
//

#ifndef CHINET_LINK_H
#define CHINET_LINK_H

#include <map>
#include <vector>
#include <bson.h>
#include <Value.h>
#include <memory>

class Value;
class Port;

class Link {
    friend Value;
    friend Port;

private:
    bson_oid_t oid;
    std::shared_ptr<Port> target;
    std::string key;

public:
    Link();
    Link(std::shared_ptr<Port> target, std::string key);
    std::shared_ptr<Value> get_target_value();
    bson_t* to_bson();
    std::string to_json();
    bool set_target(std::shared_ptr<Port> target, std::string key);
};


#endif //CHINET_LINK_H
