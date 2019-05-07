#ifndef chinet_Node_H
#define chinet_Node_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "bson.h"
#include <rttr/registration>

#include "MongoObject.h"
#include "Port.h"
#include "NodeCallback.h"


class Port;
class NodeCallback;

// Node
//====================================================================


class Node : public MongoObject{

private:

    bool node_valid_ = true;

    // Methods
    //--------------------------------------------------------------------


public:

    std::map<std::string, std::shared_ptr<Port>> input_ports;
    std::map<std::string, std::shared_ptr<Port>> output_ports;

    /// A pointer to a function that operates on an input Port instance (first argument)
    /// and writes to an output Port instance (second argument)
    std::shared_ptr<NodeCallback> callback_class;

    std::string callback;
    std::string callback_type;

    // Constructor & Destructor
    //--------------------------------------------------------------------
    Node();
    ~Node();

    // Methods
    //--------------------------------------------------------------------
    bson_t get_bson();
    bool read_from_db(const std::string &oid_string) final;
    void update();
    bool is_valid();

    // Getter
    //--------------------------------------------------------------------
    std::string get_name();
    std::vector<std::string> get_port_oids();
    std::vector<std::string> get_input_port_oids();
    std::vector<std::string> get_output_port_oids();
    std::shared_ptr<Port> get_input_port(const std::string &port_name);
    std::shared_ptr<Port> get_output_port(const std::string &port_name);

    void add_input_port(std::string key, std::shared_ptr<Port>);
    void add_output_port(std::string key, std::shared_ptr<Port>);

    // Setter
    //--------------------------------------------------------------------
    void set_callback(std::shared_ptr<NodeCallback> cb);
    void set_callback(std::string callback, std::string callback_type);

};


#endif //chinet_Node_H
