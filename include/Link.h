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

#include "Port.h"

class Port;

class Link : public MongoObject{

private:
    std::shared_ptr<Port> target;

public:

    Link();
    double get_target_value_double();
    int get_target_value_int();
    void set_target_value(double v);
    void set_target(std::shared_ptr<Port> v);

    /*
    Link(std::shared_ptr<Port> target, std::string key);
    std::shared_ptr<Value<T>> get_target_value();
    bson_t* to_bson();
    std::string to_json();
    bool set_target(std::shared_ptr<Port> target, std::string key);
     */
};


#endif //CHINET_LINK_H
