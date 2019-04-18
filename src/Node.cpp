#include <Port.h>
#include "Node.h"

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

Node::Node(const char *uri_string,
     std::string json_string_node
) : Node(uri_string){
    if(connect_to_uri(uri_string)){
        from_json(json_string_node);
    }
}

Node::Node(const char *uri_string){
    mongoc_init();
    bson_oid_init (&oid, nullptr);

    // the birth is the current time stored as a long
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    uint64_t duration = value.count();

    document = BCON_NEW(
            "_id", BCON_OID(&oid),
            "input_port_oid", BCON_OID(&oid),
            "output_port_oid", BCON_OID(&oid),
            "callback", BCON_UTF8(""),
            "callback_type", BCON_UTF8(""),
            "birth", BCON_INT64(duration),
            "death", BCON_INT64(0)
            );

    connect_to_uri(uri_string);

    std::cout << "adding node to collection" << std::endl;
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
        const char *input_port_oid,
        const char *output_port_oid
) : Node(uri_string)
{
    set_input_port(get_port(input_port_oid));
    set_output_port(get_port(output_port_oid));
}

Node::Node(
        const char *uri_string,
        const char *input_port_oid,
        const char *output_port_oid,
        std::string callback,
        std::string callback_type
) : Node(uri_string)
{
    set_input_port(get_port(input_port_oid));
    set_output_port(get_port(output_port_oid));
    set_callback(callback, callback_type);
}

// Destructor
//--------------------------------------------------------------------
Node::~Node() {
    /*
     * Release our handles and clean up mongoc
     */
    /*
    if (query) {
        bson_destroy (query);
    }
     */

    // destroy cursor, collection, session before the client they came from
    if (node_collection) {
        mongoc_collection_destroy (node_collection);
    }
    if (port_collection) {
        mongoc_collection_destroy (port_collection);
    }
    if (uri) {
        mongoc_uri_destroy (uri);
    }
    if (client) {
        mongoc_client_destroy (client);
    }
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
        database = mongoc_client_get_database (client, "db_mofa");
        /*
         * Get a handle on the collections
         */
        node_collection = mongoc_client_get_collection (client, "db_mofa", "nodes");
        port_collection = mongoc_client_get_collection (client, "db_mofa", "ports");
        return true;
    }
}

void Node::set_callback(std::shared_ptr<NodeCallback> cb){
    callback_class = cb;
}

void Node::update(){
    if((input_port == nullptr) ||
    (output_port == nullptr)) {
        return;
    }
    std::cout << callback_type << std::endl;
    switch(callback_type){
        case 1: {             // C
            std::cout << "c"  << std::endl;
            rttr::method meth = rttr::type::get_global_method(callback);
            if (meth) {
                rttr::variant return_value = meth.invoke({}, input_port, output_port);
                /*
                if (return_value.is_valid() && return_value.is_type<double>())
                    std::cout << return_value.get_value<double>() << std::endl; // 729
                */
            }
            break;
        }
        default :{
            if (callback_class != nullptr) {
                callback_class->run(input_port, output_port);
            }
        }
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
    mongoc_collection_insert_one(
            node_collection,
            document,
            nullptr, nullptr, nullptr
    );

    //append_port_to_collection(input);
    //append_port_to_collection(output);

    // input node
    // set the predecessor and update the oid
    input_port->set_predecessor(input_port->oid);
    input_port->new_oid();

    // write to the db
    if (!mongoc_collection_insert_one (port_collection, input_port->b, nullptr, nullptr, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }

    // output node
    // set the predecessor and update the oid
    output_port->set_predecessor(output_port->oid);
    output_port->new_oid();

    // write to the db
    if (!mongoc_collection_insert_one (port_collection, output_port->b, nullptr, nullptr, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }

    // update the references to the input and output ports
    bson_t *update = NULL;
    query = BCON_NEW ("_id", BCON_OID (&oid));
    update = BCON_NEW ("$set",
                       "{",
                       "input_port_oid",
                       BCON_OID (&(input_port->oid)),
                       "output_port_oid",
                       BCON_OID (&(output_port->oid)),
                       "}");
    if (!mongoc_collection_update_one (
            node_collection, query, update, NULL, NULL, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }
}

/*
bool Node::update_db(){


    // copy node

    bson_t *update = NULL;
    query = BCON_NEW ("_id", BCON_OID (&oid));
    update = BCON_NEW ("$set",
                       "{",
                       "key",
                       BCON_UTF8 ("new_value"),
                       "updated",
                       BCON_BOOL (true),
                       "}");
}
*/

bool Node::append_port_to_collection(std::shared_ptr<Port> port) {
    return mongoc_collection_insert_one(
            port_collection,
            port->b,
            nullptr, nullptr, nullptr
    );
}

bool Node::add_node_to_collection() {
}

void Node::from_json(const std::string &json_string){
    std::cout << "from_json" << std::endl;

    document = bson_new_from_json(
            (uint8_t*)json_string.c_str(),
            json_string.size(),
            &error
    );

    bson_iter_t iter;
    bson_oid_t port_oid;
    char *oid_s = new char[25];

    // set input
    if (bson_iter_init(&iter, document) &&
        bson_iter_find(&iter, "input_port_oid") &&
        BSON_ITER_HOLDS_OID(&iter)
            ) {
        port_oid = *bson_iter_oid(&iter);
        bson_oid_to_string(&port_oid, oid_s);
        std::cout << "found input_port_oid: " << oid_s << std::endl;
        input_port = get_port(&port_oid);
    }
    std::cout << "1";

    // set output
    if (bson_iter_init(&iter, document) &&
        bson_iter_find(&iter, "output_port_oid") &&
        BSON_ITER_HOLDS_UTF8(&iter)) {
        port_oid = *bson_iter_oid(&iter);
        bson_oid_to_string(&port_oid, oid_s);
        std::cout << "found output_port_oid: " << oid_s << std::endl;
        output_port = get_port(&port_oid);
    }
    std::cout << "2";

    // set callback
    std::string s_callback;
    std::string s_callback_type;
    if (bson_iter_init(&iter, document) &&
        bson_iter_find(&iter, "callback") &&
        BSON_ITER_HOLDS_UTF8(&iter)) {
        std::cout << "found callback" << std::endl;
        uint32_t l;
        const char *s = bson_iter_utf8(&iter, &l);
        s_callback = std::string(s, l);
    }
    if (bson_iter_init(&iter, document) &&
        bson_iter_find(&iter, "callback_type") &&
        BSON_ITER_HOLDS_UTF8(&iter)) {
        std::cout << "found callback_type" << std::endl;
        uint32_t l;
        const char *s = bson_iter_utf8(&iter, &l);
        s_callback_type = std::string(s, l);
    }
    std::cout << "3";
    set_callback(s_callback, s_callback_type);

    if (!mongoc_collection_insert_one (node_collection, document, nullptr, nullptr, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }
}


std::string Node::to_json(){
    if(document == nullptr){
        return "";
    } else{
        size_t len;
        char* str = bson_as_json (document, &len);
        return std::string(str, len);
    }
}


// Getter
//--------------------------------------------------------------------
std::string Node::get_name(){
    if((input_port != nullptr) &&
       (callback_class != nullptr)){
        std::string r;
        r.append(callback_class->name);
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

std::shared_ptr<Port> Node::get_port(bson_oid_t *oid_port){
    query =  BCON_NEW ("_id", BCON_OID(oid_port));
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts( port_collection, query, nullptr, nullptr);
    // read the data
    const bson_t *doc;
    auto p = std::make_shared<Port>(*oid_port);
    while (mongoc_cursor_next (cursor, &doc)) {
        p->b = bson_copy(doc);
    }
    mongoc_cursor_destroy(cursor);
    return p;
}


std::shared_ptr<Port> Node::get_port(const char* oid_string){
    // convert the string to an bson_oid_t
    bson_oid_t oid_port;
    bson_oid_init_from_string(&oid_port, oid_string);
    query =  BCON_NEW ("_id", BCON_OID(&oid_port));
    mongoc_cursor_t *cursor =
            mongoc_collection_find_with_opts(
                    port_collection,
                    query,
                    nullptr, nullptr);
    // read the data
    const bson_t *doc;
    std::shared_ptr<Port> p(new Port);
    while (mongoc_cursor_next (cursor, &doc)) {
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

std::string Node::get_output_oid(){
    bson_iter_t iter;
    if (bson_iter_init(&iter, document) &&
        bson_iter_find(&iter, "output_port_oid")
            ) {
        if (bson_iter_init (&iter, document) &&
            BSON_ITER_HOLDS_UTF8(&iter)) {
            uint32_t l;
            const char* s = bson_iter_utf8(&iter, &l);
            return std::string(s, l);
        }
    }
    return nullptr;
}

std::string Node::get_input_oid(){
    bson_iter_t iter;
    if (bson_iter_init(&iter, document) &&
    bson_iter_find(&iter, "input_port_oid")
    ) {
        if (bson_iter_init (&iter, document) &&
            BSON_ITER_HOLDS_OID(&iter)) {
            uint32_t l = 25;
            char *oid_s = new char[l];
            bson_oid_t const *s  = bson_iter_oid(&iter);
            bson_oid_to_string(s, oid_s);
            return std::string(oid_s, l);
        }
    }
    return "";
}

// Setter
//--------------------------------------------------------------------
void Node::set_input_port(std::shared_ptr<Port> input) {
    input_port = input;

    bson_iter_t iter;
    if (bson_iter_init (&iter, document) &&
        bson_iter_find (&iter, "input_port_oid") &&
        BSON_ITER_HOLDS_OID (&iter)) {
        bson_iter_overwrite_oid(&iter, &(input->oid));
        std::cout << "new input_port_oid: " << input->get_oid() << std::endl;
    }
}

void Node::set_input_port(Port* input) {
    set_input_port(input->shared_ptr());
}

void Node::set_output_port(Port* output) {
    set_output_port(output->shared_ptr());
}

void Node::set_output_port(std::shared_ptr<Port> output){
    output_port = output;
    bson_iter_t iter;
    if (bson_iter_init (&iter, document) &&
        bson_iter_find (&iter, "output_port_oid") &&
        BSON_ITER_HOLDS_OID (&iter)) {
        bson_iter_overwrite_oid(&iter, &(output->oid));
        std::cout << "new output_port_oid: " << output->get_oid() << std::endl;
    }
}

void Node::set_callback(std::string &s_callback, std::string &s_callback_type){
    this->callback = s_callback;
    if(s_callback_type == "C"){
        rttr::method meth = rttr::type::get_global_method(callback);
        if(!meth){
            std::cerr << "The class type " << callback << " does not exist." <<
                      " No callback set. " << std::endl;
        }
        this->callback_type = 1;
    } else{
        this->callback_type = 0;
    }
}