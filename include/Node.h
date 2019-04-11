#ifndef FLUOMODLIB_Node_H
#define FLUOMODLIB_Node_H

#include <string>
#include <vector>
#include <memory>
#include <map>

#include <mongoc.h>
#include <mongoc/mongoc.h>

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
    mongoc_collection_t *collection;
    bson_t *command, reply;
    bson_error_t error;

    /// An id unique to every Node instance
    bson_oid_t oid;

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
    std::shared_ptr<NodeCallback> callback;


public:

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
    ~Node();

    // Methods
    //--------------------------------------------------------------------
    void update();
    bool is_valid();
    bool write_to_db();
    bool connect_to_uri(const char* uri_string);


    // Getter
    //--------------------------------------------------------------------
    std::string get_name();
    //void* get_input_data();
    //void* get_output_data();
    std::shared_ptr<Port> get_input_port();
    std::shared_ptr<Port> get_output_port();
    std::string get_oid();

    // Setter
    //--------------------------------------------------------------------
    void set_input_port(std::shared_ptr<Port> input);
    void set_input_port(Port* input);

    void set_output_port(std::shared_ptr<Port> output);
    void set_output_port(Port* output);

    void set_callback(std::shared_ptr<NodeCallback> cb);

};


#endif //FLUOMODLIB_Node_H
