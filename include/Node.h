#ifndef FLUOMODLIB_Node_H
#define FLUOMODLIB_Node_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <chrono>

#include "bson.h"
#include <mongoc.h>
#include <mongoc/mongoc.h>
#include <rttr/registration>

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
    mongoc_database_t *database;
    mongoc_collection_t *node_collection, *port_collection;
    bson_t *query;
    bson_error_t error;

    /// An id unique to every Node instance
    bson_oid_t oid;
    bson_t *document;

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

public:
    std::string callback;
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
            const char *input_port_oid,
            const char *output_port_oid,
            std::string callback,
            std::string call_back_type
    );
    ~Node();

    // Methods
    //--------------------------------------------------------------------
    void update();
    bool is_valid();
    bool write_to_db();
    //bool update_db();
    bool connect_to_uri(const char* uri_string);
    bool append_port_to_collection(std::shared_ptr<Port> port);
    bool add_node_to_collection();
    std::string get_input_oid();
    std::string get_output_oid();

    // Getter
    //--------------------------------------------------------------------
    std::string get_name();
    //void* get_input_data();
    //void* get_output_data();
    std::shared_ptr<Port> get_port(bson_oid_t *oid_port);
    std::shared_ptr<Port> get_port(const char* oid);
    std::shared_ptr<Port> get_input_port();
    std::shared_ptr<Port> get_output_port();
    std::string get_oid();
    void from_json(const std::string &json_string);
    std::string to_json();

    // Setter
    //--------------------------------------------------------------------
    void set_input_port(std::shared_ptr<Port> input);
    void set_input_port(Port* input);

    void set_output_port(std::shared_ptr<Port> output);
    void set_output_port(Port* output);

    void set_callback(std::shared_ptr<NodeCallback> cb);
    void set_callback(std::string &callback, std::string &callback_type);

};


#endif //FLUOMODLIB_Node_H
