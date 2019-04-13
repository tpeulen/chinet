#include <NodeCallback.h>

//using namespace rttr;

struct MyStruct {
    MyStruct() {};

    void func(double) {};
    int data;
};

RTTR_REGISTRATION {
rttr::registration::class_<MyStruct>("MyStruct")
.constructor<>()
.property("data", &MyStruct::data)
.method("func", &MyStruct::func);
}



void NodeCallback::run(std::shared_ptr<Port> input, std::shared_ptr<Port> output){
    std::cout << "This print from C++" << std::endl;
}


NodeCallback::NodeCallback(std::string name){
    this->name = name;
}

