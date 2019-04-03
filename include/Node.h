#ifndef FLUOMODLIB_Node_H
#define FLUOMODLIB_Node_H

#include <string>
#include <vector>
#include <memory>
#include <NodeOperations.h>

class Node {

private:

    /// the value of the Node
    double value;

    /*!
     * Stores if an value is valid. The value of a Node that is not linked to other Nodes is valid by default. A
     * Node that is linked to other nodes is only valid if its value matches the value the is calculated by the
     * values of the input nodes and the associated function.
     */
    bool value_valid = true;

    /// a list of Nodes the are used to compute the value of the Node
    std::vector<Node*> input_ports;

    /// a list of pointers to Nodes that use this node to calculate their values
    std::vector<Node*> ouput_ports;

    /// the pointer to the function that evaluates the linked input Nodes to yield the output
    double (*eval)(std::vector<Node*> &v);

    /// the name of the Node (only used of representation)
    std::string name;

    /// if true the Node is updated on each call of get_value
    bool auto_update = true;

    /// if true a non-linked Node is not varied in optimizations and has a constant value
    bool fixed = true;


protected:

    /// creates a name for a function and a list of pointers to Nodes
    std::string make_name(std::string function_name, std::vector<Node*> v);

public:

    // Constructor
    Node(double v, std::string n);
    explicit Node(double v);
    Node();


    // Operators
    Node operator*(Node &v);
    /*
    Node operator+(Node &v);
    Node operator-(Node &v);
    Node operator/(Node &v);
    */

    void update(){
        if(is_linked()){
            value = eval(input_ports);
        }
    }

    void set_input_ports(
            std::vector<Node *> &v
    ){
        input_ports = v;
        name = make_name("", input_ports);
    }

    void set_operation(
            std::string operation_name,
            double(*pfn)(std::vector<Node *> &v)
            ){
        eval = pfn;
        name = make_name(operation_name, input_ports);
    }

    // Getter

    double get_value(){
        if(auto_update){
            update();
        }
        return value;
    }

    std::string get_name(){
        return name;
    }

    std::vector<Node*> get_input_nodes(){
        return input_ports;
    }

    // Setter

    /*!
     * Set the internal value of the Node.
     *
     * Note: For linked Nodes the 'external' value returned get_value is unaffected. Sets for independent Nodes
     * the values of the dependent nodes to invalid.
     *
     * @param v the new value of the Node
     */
    void set_value(double v){
        value = v;
        // for independent nodes set all the values of the linked Nodes to invalid
        if(!is_linked()){
            for(auto ni : ouput_ports){
                ni->value_valid = false;
            }
        }
    };


    // Methods
    bool is_linked(){
        return (!input_ports.empty());
    }

    bool is_valid(){
        if(input_ports.empty()){
            return true;
        } else{
            for(auto v : input_ports){
                if(!v->is_valid()){
                    return false;
                }
            }
            return true;
        }
    }
};



#endif //FLUOMODLIB_Node_H
