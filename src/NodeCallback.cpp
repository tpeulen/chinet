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
    std::vector<double> irf = input->value("irf");
    std::vector<double> decay = input->value("decay");
    std::vector<double> lifetimes = input->value("lifetimes");

    int start = input->value("start")[0];
    int stop = input->value("stop")[0];
    double dt = input->value("dt")[0];
    double period = input->value("period")[0];

    Functions::convolve_sum_of_exponentials_periodic(
            decay.data(), decay.size(),
            lifetimes.data(), lifetimes.size(),
            irf.data(), irf.size(),
            start, stop, dt, period
            );

    output->set_slot_value("decay", decay);
}

RTTR_REGISTRATION {
    using namespace rttr;
    registration::class_<NodeCallback>("NodeCallback").constructor<>().method("run", &NodeCallback::run);
    registration::method("NodeCallback2", &run2);
    registration::method("convolve", &convolve_sum_of_exponentials_periodic);
}
