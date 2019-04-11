//
// Created by thomas on 4/3/19.
//

#ifndef FLUOMODLIB_NODEOPERATIONS_H
#define FLUOMODLIB_NODEOPERATIONS_H

#include <rttr/registration>
#include <Port.h>
#include <Node.h>

class Node;
class Port;

// NodeCallback
//====================================================================

class NodeCallback{
public:
    std::string name;
    virtual void run(std::shared_ptr<Port>, std::shared_ptr<Port>);
    NodeCallback() {};
    NodeCallback(std::string name);
    virtual ~NodeCallback() {};
};



/*
namespace NodeOperations {

    typedef void (*eval_t)(std::vector<Port *> &input, std::vector<Port *> &output);

    void multiply(std::vector<Port *> &inputs, std::vector<Port *> &ouptputs);

    eval_t get_node_operation_multiply();

};
 */


#endif //FLUOMODLIB_NODEOPERATIONS_H
