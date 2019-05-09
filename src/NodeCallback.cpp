#include <NodeCallback.h>

//using namespace rttr;


void multiply(
        std::map<std::string, std::shared_ptr<Port>> inputs,
        std::map<std::string, std::shared_ptr<Port>> outputs){

    double mul = 1.0;
    for(auto &o : inputs){
        std::cout << o.first << ":" << o.second->get_value<double>() << std::endl;
        mul *= o.second->get_value<double>();
    }

    outputs["portC"]->set_value<double>(mul);
}

void convolve_sum_of_exponentials_periodic(
        std::map<std::string, std::shared_ptr<Port>> inputs,
        std::map<std::string, std::shared_ptr<Port>> output){
    /*
    std::vector<double> irf = input_map["irf"]->value();
    std::vector<double> decay = input_map["decay"]->value();
    std::vector<double> lifetimes = input_map["lifetimes"]->value();
    */

    /*
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
     */
}

RTTR_REGISTRATION {
    using namespace rttr;
    registration::class_<NodeCallback>("NodeCallback").constructor<>().method("run", &NodeCallback::run);
    registration::method("multiply", &multiply);
    registration::method("convolve", &convolve_sum_of_exponentials_periodic);
}
