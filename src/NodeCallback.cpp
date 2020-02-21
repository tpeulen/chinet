#include <NodeCallback.h>

//using namespace rttr;

template <typename T>
inline void mul(
        T* tmp,
        size_t &n_elements,
        const std::map<std::string, Port*> &inputs
        )
{
    for(int i=0; i<n_elements; i++) tmp[i] = 1.0;
    for(auto &o : inputs){
        T* va; int vn;
        o.second->get_value<T>(&va, &vn);
        for(int i=0; i < n_elements; i++){
            tmp[i] *= va[i];
        }
    }
}

template <typename T>
inline void add(
        T* tmp,
        size_t &n_elements,
        const std::map<std::string, Port*> &inputs
)
{
    for(int i=0; i<n_elements; i++) tmp[i] = 0.0;
    for(auto &o : inputs){
        T* va; int vn;
        o.second->get_value<T>(&va, &vn);
        for(int i=0; i < n_elements; i++){
            tmp[i] += va[i];
        }
    }
}


template <typename T>
void combine(
        std::map<std::string, Port*> &inputs,
        std::map<std::string, Port*> &outputs,
        int operation
        ){
    size_t n_elements = UINT_MAX;
    for(auto &o : inputs){
#if DEBUG
        std::clog << "Determining vector with smallest length" << std::endl;
#endif
        n_elements = MIN(n_elements, o.second->current_size());
    }
#if DEBUG
    std::clog << "Smallest length: " << n_elements << std::endl;
#endif
    auto tmp = (T*) malloc(n_elements * sizeof(T));
#if DEBUG
    std::clog << "C callback: multiply" << std::endl;
#endif
    switch(operation){
        case 0:
            add(tmp, n_elements, inputs);
            break;
        case 1:
            mul(tmp, n_elements, inputs);
            break;
        default:
            break;
    }
    if(!outputs.empty()){
        if (outputs.find("outA") == outputs.end() ) {
#if DEBUG
            std::clog << "Error: Node does not define output port with the name 'outA' " << std::endl;
#endif
            outputs.begin()->second->set_value(tmp, n_elements);
        } else {
            outputs["outA"]->set_value(tmp, n_elements);
        }
    } else{
        std::cerr << "There are no output ports." << std::endl;
    }
    free(tmp);
}


template <typename T>
void addition(
        std::map<std::string, Port*> &inputs,
        std::map<std::string, Port*> &outputs
)
{
    combine<T>(inputs, outputs, 0);
}

template <typename T>
void multiply(
        std::map<std::string, Port*> &inputs,
        std::map<std::string, Port*> &outputs
)
{
    combine<T>(inputs, outputs, 1);
}

void nothing(
        std::map<std::string, Port*> &inputs,
        std::map<std::string, Port*> &outputs){
}

void passthrough(
        std::map<std::string, Port*> &inputs,
        std::map<std::string, Port*> &outputs
        ){
    for(auto it_in = inputs.cbegin(), end_in = inputs.cend(),
            it_out = outputs.cbegin(), end_out = outputs.cend();
            it_in != end_in || it_out != end_out;)
    {
        double* va; int vn;
        it_in->second->get_value(&va, &vn);
        auto v = std::vector<double>();
        v.assign(va, va + vn);
        if(it_in != end_in) {
            ++it_in;
        } else{
            break;
        }
        if(it_out != end_out) {
            it_out->second->set_value(v.data(), v.size());
            ++it_out;
        } else{
            break;
        }
    }
}

void convolve_sum_of_exponentials_periodic(
        std::map<std::string, Port*> &inputs,
        std::map<std::string, Port*> &outputs
        ){
    // Make sure that all inputs are set

    int n_irf; double* irf;
    inputs["irf"]->get_value(&irf, &n_irf);
    int n_decay; double* decay;
    inputs["decay"]->get_value(&decay, &n_decay);
    int n_lifetimes; double* lifetimes;
    inputs["lifetimes"]->get_value(&lifetimes, &n_lifetimes);

//    long start = inputs["start"]->get_value<long>();
//    long stop = inputs["stop"]->get_value<long>();
//    double dt = inputs["dt"]->get_value<double>();
//    long period = inputs["period"]->get_value<long>();
//
//
//    Functions::convolve_sum_of_exponentials_periodic(
//            decay.data(), decay.size(),
//            lifetimes.data(), lifetimes.size(),
//            irf.data(), irf.size(),
//            start, stop, dt, period
//            );
//
//    output->set_slot_value("decay", decay);
}

void AV(
        std::map<std::string, Port*> &inputs,
        std::map<std::string, Port*> &output){
    /*
    auto g = dyeDensity(
            
            );
    */
}

RTTR_REGISTRATION {
    using namespace rttr;
    registration::class_<NodeCallback>("NodeCallback").constructor<>().method("run", &NodeCallback::run);
    registration::method("multiply_double", &multiply<double>);
    registration::method("multiply_int", &multiply<long>);
    registration::method("addition_double", &addition<double>);
    registration::method("addition_int", &addition<long>);
    registration::method("nothing", &nothing);
    registration::method("passthrough", &passthrough);
    registration::method("convolve", &convolve_sum_of_exponentials_periodic);
}
