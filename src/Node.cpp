//
// Created by thomas on 4/2/19.
//

#include "Node.h"

// Constructor


Node::Node(std::string n) :
name(n)
{}


Node::Node() :
Node("")
{}


std::string Node::make_name(const std::string function_name, std::vector<Port *> v) {
    std::string r;
    r.append(function_name);
    r.append("(");
    for(auto vi : v){
        r.append(vi->name);
        r.append(",");
    }
    r.append(")");
    return r;
}

