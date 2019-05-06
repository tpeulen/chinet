//
// Created by thomas on 4/3/19.
//

#ifndef chinet_NODECALLBACK_H
#define chinet_NODECALLBACK_H

#include <rttr/registration>
#include <Port.h>
#include <Node.h>
#include <Functions.h>

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
};




#endif //chinet_NODECALLBACK_H