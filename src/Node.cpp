#include <Port.h>
#include "Node.h"

/*
 * Required to initialize libmongoc's internals
 */
void mongoc_init(void);


// Constructor
//--------------------------------------------------------------------
Node::Node() {
    bson_oid_init (&oid, nullptr);

    // Database
    //----------------------------------------------------------------
    const char *uri_string = "mongodb://localhost:27017";
    uri = mongoc_uri_new_with_error (uri_string, &error);
    if (!uri) {
        fprintf (stderr,
                 "failed to parse URI: %s\n"
                 "error message:       %s\n",
                 uri_string,
                 error.message);
    } else{
        printf("Node connected to URI: %s\n", uri_string);
    }

    /*
    * Create a new client instance
    */
    client = mongoc_client_new_from_uri (uri);

    /*
    * Register the application name so we can track it in the profile logs
    * on the server. This can also be done from the URI (see other examples).
    */
    mongoc_client_set_appname (client, "mofa-node");

    /*
     * Get a handle on the database "db_name" and collection "coll_name"
     */
    database = mongoc_client_get_database (client, "db_mofa");
    collection = mongoc_client_get_collection (client, "db_mofa", "nodes");

    /*
     * Do work. This example pings the database, prints the result as JSON and
     * performs an insert
     */

    command = BCON_NEW ("ping", BCON_INT32 (1));

    char *str;
    bool retval;
    retval = mongoc_client_command_simple (
            client, "admin", command, NULL, &reply, &error);

    if (!retval) {
        fprintf (stderr, "%s\n", error.message);
    }

    str = bson_as_json (&reply, NULL);
    printf ("%s\n", str);
    bson_free (str);

}


Node::Node(std::shared_ptr<Port> input, std::shared_ptr<Port> output, std::shared_ptr<NodeCallback> callback) :
Node()
{
    set_input_port(input);
    set_output_port(output);
    set_callback(callback);
}

// Destructor
//--------------------------------------------------------------------
Node::~Node() {
    bson_destroy (&reply);
    bson_destroy (command);

    /*
     * Release our handles and clean up mongoc
     */
    mongoc_collection_destroy (collection);
    mongoc_database_destroy (database);
    mongoc_uri_destroy (uri);
    mongoc_client_destroy (client);
    mongoc_cleanup ();
}


// Methods
//--------------------------------------------------------------------

void Node::set_callback(std::shared_ptr<NodeCallback> cb){
    callback = cb;
}

void Node::update(){
    if((input_port != nullptr) &&
    (output_port != nullptr) &&
    (callback != nullptr)) {
        callback->run(input_port, output_port);
    }
}

bool Node::is_valid(){
    if(input_port == nullptr){
        return true;
    } else{
        return node_valid;
    }
}

// Getter
//--------------------------------------------------------------------
std::string Node::get_name(){
    if((input_port != nullptr) &&
       (callback != nullptr)){
        std::string r;
        r.append(callback->name);
        r.append("(");
        for(auto n : input_port->get_slot_keys()){
            r.append(n);
        }
        r.append(")");
        return r;
    } else{
        return "NA";
    }
}

std::shared_ptr<Port> Node::get_input_port(){
    return this->input_port;
}

std::shared_ptr<Port> Node::get_output_port(){
    return this->output_port;
}


// Setter
//--------------------------------------------------------------------
void Node::set_input_port(std::shared_ptr<Port> input) {
    this->input_port = input;
    this->output_port = input;
    if (!mongoc_collection_insert_one (collection, input->b, NULL, NULL, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }
}

void Node::set_input_port(Port* input) {
    set_input_port(input->shared_ptr());
}

void Node::set_output_port(std::shared_ptr<Port> output){
    this->output_port = output;
}

void Node::set_output_port(Port* output) {
    set_output_port(output->shared_ptr());
}
