#include <NodeCallback.h>

//using namespace rttr;


void NodeCallback::run(std::shared_ptr<Port> input, std::shared_ptr<Port> output){
    std::cout << "This print from C++" << std::endl;
}


NodeCallback::NodeCallback(std::string name){
    this->name = name;
}

void run2(std::shared_ptr<Port> input, std::shared_ptr<Port> output){
    /*
    if (return_value.is_valid() && return_value.is_type<double>())
        std::cout << return_value.get_value<double>() << std::endl; // 729
    */

    std::cout << "This is run2 from C++" << std::endl;
    for(auto key : input->keys()){
        std::cout << key;
    }
    double test = input->value("slotA1")[0] * input->value("slotA2")[0];
    std::cout << test << std::endl;
}

void convolve_sum_of_exponentials_periodic(std::shared_ptr<Port> input, std::shared_ptr<Port> output){

    //Functions::convolve_sum_of_exponentials_periodic();
}

RTTR_REGISTRATION {
    using namespace rttr;
    registration::class_<NodeCallback>("NodeCallback").constructor<>().method("run", &NodeCallback::run);
    registration::method("NodeCallback2", &run2);
    registration::method("NodeCallback2", &convolve_sum_of_exponentials_periodic);
}
