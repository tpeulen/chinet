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

#include <Functions.h>
#include <Port.h>
#include <NodeCallback.h>


class Port;
class NodeCallback;

// Node
//====================================================================

class Node {
    friend Port;

private:
    // Database stuff
    //------------------
    mongoc_uri_t *uri;
    mongoc_client_t *client;
    mongoc_collection_t *node_collection, *port_collection;
    bson_error_t error;

    /// An id unique to every Node instance
    const bson_t *document;

    bson_oid_t oid;
    bson_oid_t input_port_oid;
    bson_oid_t output_port_oid;

    /*!
     * Stores if an value is valid. The value of a Node that is not linked to other Nodes is valid by default. A
     * Node that is linked to other nodes is only valid if its value matches the value the is calculated by the
     * values of the input nodes and the associated function.
     */
    bool node_valid = true;

    /// A pointer to a instance of a Port that serves as an input
    std::shared_ptr<Port> input_port;

    /// A pointer to a instance of a Port that serves as an output
    std::shared_ptr<Port> output_port;

    /// A pointer to a function that operates on an input Port instance (first argument)
    /// and writes to an output Port instance (second argument)
    std::shared_ptr<NodeCallback> callback_class;

    // Methods
    //--------------------------------------------------------------------
    std::shared_ptr<Port> get_port(bson_oid_t *oid_port);
    std::shared_ptr<Port> get_port(const char* oid);

public:
    std::string callback;
    std::string callback_type_s;
    int callback_type;

    // Constructor & Destructor
    //--------------------------------------------------------------------
    Node();
    Node(const char *uri_string);
    Node(std::shared_ptr<Port> input,
            std::shared_ptr<Port> output,
            std::shared_ptr<NodeCallback> callback
            );
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
    Node(
            const char *uri_string,
            const char * oid_s,
            const char *input_port_oid,
            const char *output_port_oid,
            std::string callback,
            std::string call_back_type
    );

    Node(
            const char *uri_string,
            std::string oid_s,
            std::shared_ptr<Port> input,
            std::shared_ptr<Port> output,
            std::string callback,
            std::string callback_type
    );

    ~Node();

    // Methods
    //--------------------------------------------------------------------
    void update();
    bool is_valid();
    /*
    std::string get_input_oid();
    std::string get_output_oid();
    */

    void from_json(const std::string &json_string);
    void from_bson(const bson_t *document);
    void from_oid(bson_oid_t *oid_doc);
    void from_oid(const std::string oid_doc);
    std::string to_json();

    // DB methods
    bool connect_to_uri(const char* uri_string);
    bool write_input_port_to_db();
    bool write_output_port_to_db();
    bool write_ports_to_db();
    bool write_node_to_db();
    bool write_to_db();
    bool append_ports_in_db();
    void init_node(
            const char *uri_string,
            std::string oid_s,
            std::shared_ptr<Port> input,
            std::shared_ptr<Port> output,
            std::string callback,
            std::string call_back_type
    );

    // Getter
    //--------------------------------------------------------------------
    std::string get_name();
    std::shared_ptr<Port> get_input_port();
    std::shared_ptr<Port> get_output_port();
    std::string get_oid();
    const bson_t* get_document();


    // Setter
    //--------------------------------------------------------------------
    void set_input_port(std::shared_ptr<Port> input);
    void set_input_port(Port* input);

    void set_output_port(std::shared_ptr<Port> output);
    void set_output_port(Port* output);

    void set_callback(std::shared_ptr<NodeCallback> cb);
    void set_callback(std::string &callback, std::string &callback_type);

};


#endif //chinet_Node_H
