//
// Created by thomas on 4/3/19.
//

#ifndef FLUOMODLIB_PORT_H
#define FLUOMODLIB_PORT_H

#include <string>
#include <Node.h>

class Node;

class Port {

    friend Node;

private:
    static unsigned int sNextId;
    size_t id;
    std::string name;
    double value;
    Node* node;
    Port* input = nullptr;
    std::vector<Port*> ouputs;

public:

    Port(){
        id = sNextId++;
    }

    void link_to(Port* port){
        input = port;
        port->ouputs.push_back(port);
    }

    double get_value(){
        if(input == nullptr){
            return value;
        } else{
            return input->get_value();
        }
    }

    void set_value(double v){
        value = v;
    }


    std::string get_name(){
        return name;
    }

    void set_name(std::string &v){
        name = v;
    }

    bool is_valid();

};


#endif //FLUOMODLIB_PORT_H
