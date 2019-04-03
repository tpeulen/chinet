//
// Created by thomas on 4/3/19.
//

#include <include/NodeOperations.h>


namespace NodeOperations {

    double multiply(std::vector<Node *> &v) {
        double r = 1.0;
        for (auto vi : v) {
            if (vi != nullptr) {
                r *= vi->get_value();
            }
        }
        return r;
    }

    eval_t get_node_operation_multiply(){
        return multiply;
    }

};
