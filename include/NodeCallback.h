//
// Created by thomas on 4/3/19.
//

#ifndef chinet_NODECALLBACK_H
#define chinet_NODECALLBACK_H

#include <functional>
#include <rttr/registration>
#include <Port.h>
#include <CNode.h>
#include <Functions.h>
#include <FlexLabel/FlexLabel.h>

class Port;

// NodeCallback
//====================================================================

class NodeCallback{

public:
    
    virtual void run(
            std::map<std::string, std::shared_ptr<Port>>,
            std::map<std::string, std::shared_ptr<Port>>
            ){
        std::cout << "This print by NodeCallback class from C++" << std::endl;
    }

    NodeCallback() = default;
    virtual ~NodeCallback() {};
};


#endif //chinet_NODECALLBACK_H