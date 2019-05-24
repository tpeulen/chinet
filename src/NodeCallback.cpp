#include <NodeCallback.h>

//using namespace rttr;

void multiply(
        const std::map<std::string, std::shared_ptr<Port>> &inputs,
        std::map<std::string, std::shared_ptr<Port>> &outputs){

    std::vector<double> mul;
    mul.resize(inputs.size());

    int i = 0;
    for(auto &o : inputs){
        #if CHINET_DEBUG
        std::cout << o.first << std::endl;
        #endif
        double m = 1.0;
        for(auto &v : o.second->get_value()){
            #if CHINET_DEBUG
            std::cout << v << ":" << std::endl;
            #endif
            m *= v;
        }
        mul[i] = m;
        i++;
    }
    if (outputs.find("outA") == outputs.end() ) {
        std::cerr << "Error: Node does not define output port with the name 'outA' " << std::endl;
    } else {
        outputs["outA"]->set_value(mul);
    }
}


void nothing(
        const std::map<std::string, std::shared_ptr<Port>> &inputs,
        const std::map<std::string, std::shared_ptr<Port>> &outputs){
}

void passthrough(
        const std::map<std::string, std::shared_ptr<Port>> &inputs,
        const std::map<std::string, std::shared_ptr<Port>> &outputs){

    for(auto it_in = inputs.cbegin(), end_in = inputs.cend(),
            it_out = outputs.cbegin(), end_out = outputs.cend();
            it_in != end_in || it_out != end_out;)
    {
        auto v = it_in->second->get_value();
        if(it_in != end_in) {
            ++it_in;
        } else{
            break;
        }

        if(it_out != end_out) {
            it_out->second->set_value(v);
            ++it_out;
        } else{
            break;
        }
    }
}


void convolve_sum_of_exponentials_periodic(
        const std::map<std::string, std::shared_ptr<Port>> &inputs,
        std::map<std::string, std::shared_ptr<Port>> &output){
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

void AV(
        const std::map<std::string, std::shared_ptr<Port>> &inputs,
        std::map<std::string, std::shared_ptr<Port>> &output){
    /*
    auto g = dyeDensity(
            
            );
    */
}

RTTR_REGISTRATION {
    using namespace rttr;
    registration::class_<NodeCallback>("NodeCallback").constructor<>().method("run", &NodeCallback::run);
    registration::method("multiply", &multiply);
    registration::method("nothing", &nothing);
    registration::method("passthrough", &passthrough);
    registration::method("convolve", &convolve_sum_of_exponentials_periodic);
}
