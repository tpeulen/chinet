//
// Created by thomas on 4/3/19.
//

#include <NodeCallback.h>


void NodeCallback::run(std::shared_ptr<Port> input, std::shared_ptr<Port> output){
    std::cout << "This print from C++" << std::endl;
}


NodeCallback::NodeCallback(std::string name){
    this->name = name;
};



namespace NodeOperations {
/*
    void multiply(std::vector<Port *> &inputs, std::vector<Port *> &outputs) {
        double r = 1.0;
        for (auto vi : inputs) {
            if (vi != nullptr) {
                r *= vi->get_value();
            }
        }
        outputs[0]->set_value(r);
    }

    eval_t get_node_operation_multiply(){
        return multiply;
    }
*/
}
