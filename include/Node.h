#ifndef chinet_Node_H
#define chinet_Node_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <chrono>

#include "bson.h"
#include <mongoc.h>
#include <mongoc/mongoc.h>
#include <rttr/registration>

#include "MongoObject.h"
#include "Functions.h"
#include "Port.h"
#include "NodeCallback.h"


class Port;
class NodeCallback;

// Node
//====================================================================


class Node : public MongoObject{

private:

    /// A pointer to a function that operates on an input Port instance (first argument)
    /// and writes to an output Port instance (second argument)
    std::shared_ptr<NodeCallback> callback_class;

    // Methods
    //--------------------------------------------------------------------

public:
    std::map<std::string, std::shared_ptr<Port>> input_ports;
    std::map<std::string, std::shared_ptr<Port>> output_ports;

    std::string callback;
    std::string callback_type_str;
    int callback_type;

    // Constructor & Destructor
    //--------------------------------------------------------------------
    Node();
    ~Node();

    // Methods
    //--------------------------------------------------------------------
    bson_t get_bson();
    bool read_from_db(const std::string &oid_string) final;

    // Getter
    //--------------------------------------------------------------------
    std::string get_name();
    std::vector<std::string> get_port_oids();
    std::vector<std::string> get_input_port_oids();
    std::vector<std::string> get_output_port_oids();

    void add_input_port(std::string key, std::shared_ptr<Port>);
    void add_output_port(std::string key, std::shared_ptr<Port>);

    // Setter
    //--------------------------------------------------------------------
    void set_callback(std::shared_ptr<NodeCallback> cb);
    void set_callback(std::string &callback, std::string &callback_type);

    // std::shared_ptr<Port> get_port(bson_oid_t *oid_port);
    // std::shared_ptr<Port> get_port(const char* oid);

    /*
    void update();
    bool is_valid();

    void from_json(const std::string &json_string);
    void from_bson(const bson_t *document);
    void from_oid(bson_oid_t *oid_doc);
    */


    /*
    void init_node(
            const char *uri_string,
            std::string oid_s,
            std::shared_ptr<Port> input,
            std::shared_ptr<Port> output,
            std::string callback,
            std::string call_back_type
    );
     */

    //bool node_valid = true;

    /*
    Node(const char *uri_string,
            std::shared_ptr<Port> input,
            std::shared_ptr<Port> output,
            std::shared_ptr<NodeCallback> callback
            );
    Node(const char *uri_string,
         std::string json_string_node
    );
    Node(const char *uri_string,
         const char *input_port_oid,
         const char *output_port_oid
    );
     */

};


#endif //chinet_Node_H
