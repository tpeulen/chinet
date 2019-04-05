//
// Created by thomas on 4/2/19.
//

#include <include/Port.h>
#include "Node.h"

int Node::sNextId = 0;

// Constructor


Node::Node(std::string n) :
name(n)
{
    id = sNextId++;
}


Node::Node() :
Node("")
{
}

// Methods
std::string Node::make_name(const std::string function_name) {
    std::string r;
    r.append(function_name);
    r.append("(");
    r.append(input->get_name());
    r.append(")");
    return r;
}


void Node::set_operation(std::string &operation_name, void(*pfn)(Port &, Port &)){
    eval = pfn;
    name = make_name(operation_name);
}


void Node::update(){
    if(input != nullptr){
        eval(*input, *output);
    }
}


bool Node::is_valid(){
    if(input == nullptr){
        return true;
    } else{
        return node_valid;
    }
}


void Node::link_input_to(std::shared_ptr<Port> &port){
    this->input = port;
    port->targets.push_back(input);
}

// Getter
std::string Node::get_name(){
    return this->name;
}

std::shared_ptr<Port> Node::get_input_port(){
    return this->input;
}

std::shared_ptr<Port> Node::get_output_port(){
    return this->output;
}


// Setter
void Node::set_input_port(std::shared_ptr<Port> input) {
    this->input = input;
    this->name = make_name("");
    this->output = input;
}

void Node::set_input_port(Port* input) {
    set_input_port(input->shared_ptr());
}

void Node::set_output_port(std::shared_ptr<Port> output){
    this->output = output;
}

void Node::set_output_port(Port* output) {
    set_output_port(output->shared_ptr());
}
