//
// Created by thomas on 4/3/19.
//

#ifndef FLUOMODLIB_PORT_H
#define FLUOMODLIB_PORT_H

#include <string>
#include <Node.h>

class Node;

class Port {

private:

    std::string name;
    double value;
    Node* node;

public:

    double get_value();

    void set_value(double v){
        value = v;
    }


    std::string get_name(){
        return name;
    }

    void set_name(std::string &v){
        name = v;
    }

    bool is_valid(){
        return node->is_linked();
    }

};


#endif //FLUOMODLIB_PORT_H
