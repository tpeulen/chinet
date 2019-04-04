#ifndef FLUOMODLIB_Node_H
#define FLUOMODLIB_Node_H

#include <string>
#include <vector>
#include <memory>
#include <Port.h>
#include <map>
#include <NodeOperations.h>


class Port;

class Node {

    static int sNextId;

private:

    /// An id unique to every Node instance
    size_t id;

    /// The name of the Node instance (only used of representation)
    std::string name;

    /*!
     * Stores if an value is valid. The value of a Node that is not linked to other Nodes is valid by default. A
     * Node that is linked to other nodes is only valid if its value matches the value the is calculated by the
     * values of the input nodes and the associated function.
     */
    bool node_valid = true;

    /// A pointer to a instance of a Port that serves as an input
    std::shared_ptr<Port> input;

    /// A pointer to a instance of a Port that serves as an output
    std::shared_ptr<Port> output;

    /// A pointer to a function that operates on an input Port instance (first argument)
    /// and writes to an output Port instance (second argument)
    void (*eval)(Port &, Port &);

    /// creates a name for a function and a list of pointers to Nodes
    std::string make_name(std::string function_name);

public:

    // Constructor
    Node(std::string n);
    Node();

    // Methods
    void update();

    void set_operation(
            std::string &operation_name,
            void(*pfn)(Port &, Port &)
            );

    void link_input_to(std::shared_ptr<Port> &port);

    bool is_valid();

    // Getter
    std::string get_name();
    std::shared_ptr<Port> get_input_port();
    std::shared_ptr<Port> get_output_port();

    // Setter
    void set_input_port(std::shared_ptr<Port> input);
    void set_input_port(Port* input);

    void set_output_port(std::shared_ptr<Port> output);
    void set_output_port(Port* output);

};


#endif //FLUOMODLIB_Node_H
