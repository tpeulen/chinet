#include <Port.h>
#include "Node.h"


// Constructor
//--------------------------------------------------------------------
Node::Node() {
    bson_oid_init (&oid, nullptr);
    // Database
    //--------------------------------------------------------------------
    const char *uri_string = "mongodb://localhost:27017";
    mongoc_uri_t *uri;
    mongoc_client_t *client;
    mongoc_database_t *database;
    mongoc_collection_t *collection;
    bson_t *command, reply, *insert;
    bson_error_t error;
    char *str;
    bool retval;
    mongoc_init ();
}


Node::Node(std::shared_ptr<Port> input, std::shared_ptr<Port> output, std::shared_ptr<NodeCallback> callback) :
Node()
{
    set_input_port(input);
    set_output_port(output);
    set_callback(callback);
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
