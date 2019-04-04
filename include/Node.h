#ifndef FLUOMODLIB_Node_H
#define FLUOMODLIB_Node_H

#include <string>
#include <vector>
#include <memory>
#include <Port.h>
#include <map>
#include <NodeOperations.h>


class Node {

private:

    /*!
     * Stores if an value is valid. The value of a Node that is not linked to other Nodes is valid by default. A
     * Node that is linked to other nodes is only valid if its value matches the value the is calculated by the
     * values of the input nodes and the associated function.
     */
    bool node_valid = true;

    /// a list of Nodes the are used to compute the value of the Node
    std::vector<Port *> input_ports;

    /// a list of pointers to Nodes that use this node to calculate their values
    std::vector<Port *> output_ports;

    /// the pointer to a function that evaluates an input vector and writes to the output vector
    void (*eval)(std::vector<Port*> &inputs, std::vector<Port*> &outputs);

    /// the name of the Node (only used of representation)
    std::string name;


protected:

    /// creates a name for a function and a list of pointers to Nodes
    std::string make_name(std::string function_name, std::vector<Port *> v);

public:

    // Constructor
    Node(std::string n);
    Node();

    void update(){
        if(!input_ports.empty()){
            eval(input_ports, output_ports);
        }
    }

    void set_operation(
            std::string operation_name,
            void(*pfn)(std::vector<Port *> &inputs, std::vector<Port *> &outputs)
            ){
        eval = pfn;
        name = make_name(operation_name, input_ports);
    }

    // Getter
    std::string get_name(){
        return name;
    }

    std::vector<Port *> get_input_ports(){
        return input_ports;
    }

    std::vector<Port *> get_output_ports(){
        return output_ports;
    }


    // Setter
    void set_input_ports(std::vector<Port *> &inputs){
        input_ports = inputs;
        name = make_name("", input_ports);
    }

    void set_output_ports(std::vector<Port *> &outputs){
        output_ports = outputs;
    }

    // Methods

    bool is_valid(){
        if(input_ports.empty()){
            return true;
        } else{
            for(auto v : input_ports){
                if(!v->is_valid()){
                    node_valid = false;
                    break;
                }
            }
            return node_valid;
        }
    }
};


#endif //FLUOMODLIB_Node_H
