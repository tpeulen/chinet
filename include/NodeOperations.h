//
// Created by thomas on 4/3/19.
//

#ifndef FLUOMODLIB_NODEOPERATIONS_H
#define FLUOMODLIB_NODEOPERATIONS_H

#include <include/Node.h>

class Node;

namespace NodeOperations {

    typedef double (*eval_t)(std::vector<Node*> &v);

    double multiply(std::vector<Node *> &v);

    eval_t get_node_operation_multiply();

};


#endif //FLUOMODLIB_NODEOPERATIONS_H
