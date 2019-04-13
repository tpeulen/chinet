#include <Port.h>
#include "Node.h"

/*
 * Required to initialize libmongoc's internals
 */
void mongoc_init(void);


// Constructor
//--------------------------------------------------------------------
Node::Node(
        const char *uri_string,
        std::shared_ptr<Port> input,
        std::shared_ptr<Port> output,
        std::shared_ptr<NodeCallback> callback
        ) : Node(uri_string)
        {
    set_input_port(input);
    set_output_port(output);
    set_callback(callback);
}


Node::Node(const char *uri_string){
    bson_oid_init (&oid, nullptr);
    connect_to_uri(uri_string);
}

Node::Node() :
Node("mongodb://localhost:27017")
{
}

Node::Node(
        std::shared_ptr<Port> input,
        std::shared_ptr<Port> output,
        std::shared_ptr<NodeCallback> callback
        ) : Node("mongodb://localhost:27017")
        {
    set_input_port(input);
    set_output_port(output);
    set_callback(callback);
}

Node::Node(
        const char *uri_string,
        std::string input_port_oid,
        std::string output_port_oid
) : Node(uri_string)
{
    set_input_port(get_port(input_port_oid));
    set_output_port(get_port(output_port_oid));
}

// Destructor
//--------------------------------------------------------------------
Node::~Node() {
    /*bson_destroy (&reply);
    bson_destroy (command);
*/
    /*
     * Release our handles and clean up mongoc
     */
    mongoc_collection_destroy (node_collection);
    mongoc_database_destroy (database);
    mongoc_uri_destroy (uri);
    mongoc_client_destroy (client);
    mongoc_cleanup ();
}


// Methods
//--------------------------------------------------------------------

bool Node::connect_to_uri(const char* uri_string){
    // Database
    //----------------------------------------------------------------
    uri = mongoc_uri_new_with_error (uri_string, &error);
    if (!uri) {
        fprintf (stderr,
                 "failed to parse URI: %s\n"
                 "error message:       %s\n",
                 uri_string,
                 error.message);
        return false;
    } else{
        fprintf(stderr, "Node connected to URI: %s\n", uri_string);
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
        node_collection = mongoc_client_get_collection (client, "db_mofa", "nodes");
        port_collection = mongoc_client_get_collection (client, "db_mofa", "ports");
        return true;
    }
}

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

bool Node::write_to_db(){
    if (!mongoc_collection_insert_one (node_collection, input_port->b, NULL, NULL, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }
    if (!mongoc_collection_insert_one (node_collection, output_port->b, NULL, NULL, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }
}

bool Node::append_port_to_collection(std::shared_ptr<Port> port) {
    return mongoc_collection_insert_one(
            port_collection,
            port->b,
            nullptr, nullptr, nullptr
    );
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
            r.append(",");
        }
        r.append(")");
        return r;
    } else{
        return "NA";
    }
}


std::shared_ptr<Port> Node::get_port(const std::string oid_string){
    // convert the string to an bson_oid_t
    bson_oid_t oid_port;
    bson_oid_init_from_string(&oid_port, oid_string.c_str());

    // find the oid_pinter for the oid_string in the DB
    bson_t *query =  BCON_NEW ("_id", BCON_OID(&oid_port));
    mongoc_cursor_t *cursor =
            mongoc_collection_find_with_opts(
                    port_collection,
                    query,
                    nullptr, nullptr);

    // read the data
    const bson_t *doc;
    char *str;
    std::shared_ptr<Port> p(new Port);
    while (mongoc_cursor_next (cursor, &doc)) {
        /*
        std::cout << ".";
        str = bson_as_canonical_extended_json (doc, NULL);
        fprintf (stdout, "%s\n", str);
        bson_free (str);
         */
        p->b = bson_copy(doc);
    }
    mongoc_cursor_destroy(cursor);
    return p;
}

std::shared_ptr<Port> Node::get_output_port(){
    return this->output_port;
}

std::shared_ptr<Port> Node::get_input_port(){
    return this->input_port;
}

std::string Node::get_oid(){
    char * oid_s = new char[25];
    bson_oid_to_string(&oid, oid_s);
    return std::string(oid_s);
}

// Setter
//--------------------------------------------------------------------
void Node::set_input_port(std::shared_ptr<Port> input) {
    input_port = input;
    append_port_to_collection(input);
}

void Node::set_input_port(Port* input) {
    set_input_port(input->shared_ptr());
}

void Node::set_output_port(Port* output) {
    set_output_port(output->shared_ptr());
}

void Node::set_output_port(std::shared_ptr<Port> output){
    output_port = output;
    append_port_to_collection(output);
}

void Node::set_callback(std::string &callback, std::string &callback_type){
    // TODO
}