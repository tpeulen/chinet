//
// Created by thomas on 4/2/19.
//

#include <include/Node.h>
#include "Node.h"

// Constructor
Node::Node()
{}


Node::Node(double v, std::string n)
:
value(v),
name(n)
{}


Node::Node(double v) :
Node(v, std::string(" "))
{}


// Operators
Node Node::operator*(Node &v){
    Node r;
    std::vector<Node *> in{this, &v};
    r.set_input_ports(in);
    r.set_operation("multiply", NodeOperations::multiply);
    ouput_ports.push_back(&r);
    v.ouput_ports.push_back(&r);
    return r;
}


std::string Node::make_name(const std::string function_name, std::vector<Node *> v) {
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

