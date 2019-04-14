//
// Created by thomas on 4/3/19.
//

#ifndef FLUOMODLIB_NODECALLBACK_H
#define FLUOMODLIB_NODECALLBACK_H

#include <rttr/registration>
#include <Port.h>
#include <Node.h>

class Node;
class Port;

// NodeCallback
//====================================================================

class NodeCallback{

public:
    std::string name;
    virtual void run(std::shared_ptr<Port>, std::shared_ptr<Port>);
    NodeCallback() {};
    NodeCallback(std::string name);
    virtual ~NodeCallback() {};
    //RTTR_ENABLE();
};




#endif //FLUOMODLIB_NODECALLBACK_H