//
// Created by thomas on 4/22/19.
//

#ifndef CHINET_LINK_H
#define CHINET_LINK_H

#include <Value.h>
#include <memory>
#include <cmath>

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

};


#endif //CHINET_LINK_H
