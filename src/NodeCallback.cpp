#include <NodeCallback.h>

//using namespace rttr;


void NodeCallback::run(std::shared_ptr<Port> input, std::shared_ptr<Port> output){
    std::cout << "This print from C++" << std::endl;
}


NodeCallback::NodeCallback(std::string name){
    this->name = name;
}

void run2(std::shared_ptr<Port> input, std::shared_ptr<Port> output){
    std::cout << "This p22222rint from C++" << std::endl;
}

RTTR_REGISTRATION {
    using namespace rttr;
    registration::class_<NodeCallback>("NodeCallback")
            .constructor<>()
            .method("run", &NodeCallback::run);

    registration::method("NodeCallback2", &run2);

}
