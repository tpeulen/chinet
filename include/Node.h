#ifndef chinet_Node_H
#define chinet_Node_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <rttr/registration>

#include "MongoObject.h"
#include "Port.h"
#include "NodeCallback.h"


// Node
//====================================================================


class Port;
class NodeCallback;
class Node : public MongoObject{

private:

    bool node_valid_ = true;
    std::map<std::string, std::shared_ptr<Port>> input_ports;
    std::map<std::string, std::shared_ptr<Port>> output_ports;

    // Methods
    //--------------------------------------------------------------------

public:

    /// A pointer to a function that operates on an input Port instance (first argument)
    /// and writes to an output Port instance (second argument)
    std::shared_ptr<NodeCallback> callback_class;

    std::string callback;
    std::string callback_type;

    // Constructor & Destructor
    //--------------------------------------------------------------------
    Node();
    Node(std::string name);

    Node(std::map<std::string, std::shared_ptr<Port>> input_ports,
         std::map<std::string, std::shared_ptr<Port>> output_ports);
    /*
    Node(std::map<std::string, double> input_ports,
         std::map<std::string, double> output_ports);
    */
    ~Node();

    // Methods
    //--------------------------------------------------------------------
    bool read_from_db(const std::string &oid_string) final;
    void evaluate();
    bool is_valid();
    bool write_to_db() final;

    // Getter
    //--------------------------------------------------------------------
    bson_t get_bson();
    std::string get_name();
    std::vector<std::string> get_port_oids();

    std::vector<std::string> get_input_port_oids();
    std::vector<std::string> get_output_port_oids();

    std::map<std::string, std::shared_ptr<Port>> get_input_ports();
    std::map<std::string, std::shared_ptr<Port>> get_output_ports();

    std::shared_ptr<Port> get_input_port(const std::string &port_name);
    std::shared_ptr<Port> get_output_port(const std::string &port_name);

    void add_input_port(std::string key, std::shared_ptr<Port>);
    void add_output_port(std::string key, std::shared_ptr<Port>);

    // Setter
    //--------------------------------------------------------------------
    void set_callback(std::shared_ptr<NodeCallback> cb);
    void set_callback(std::string callback, std::string callback_type);

    // Operators
    //--------------------------------------------------------------------
    std::shared_ptr<Port> operator[](std::string key){
        try {
            return input_ports.at(key);
        }
        catch (std::out_of_range e){
            try {
                return output_ports.at(key);
            }
            catch (std::out_of_range e){
                return nullptr;
            }
        }
    };


};


#endif //chinet_Node_H
