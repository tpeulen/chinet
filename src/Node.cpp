#include <Port.h>
#include "Node.h"

// Constructor
//--------------------------------------------------------------------


Node::Node(
        const char *uri_string,
        std::string oid_s,
        std::shared_ptr<Port> input,
        std::shared_ptr<Port> output,
        std::string callback,
        std::string callback_type
){
    std::clog << "Node:";
    if(connect_to_uri(uri_string)){
        std::cout << "connected:";
    }
    init_node(uri_string, oid_s,
            input, output,
            callback, callback_type);
    write_ports_to_db();
}

Node::Node(
        const char *uri_string,
        const char *oid_s,
        const char *input_port_oid,
        const char *output_port_oid,
        std::string callback,
        std::string callback_type
){
    if(connect_to_uri(uri_string)){
        std::cout << "connected:";
    }
    init_node(
            uri_string, oid_s,
            get_port(input_port_oid), get_port(output_port_oid),
            callback, callback_type
            );
}

Node::Node(
        const char *uri_string,
        std::shared_ptr<Port> input,
        std::shared_ptr<Port> output,
        std::shared_ptr<NodeCallback> callback
        )
{
    if(connect_to_uri(uri_string)){
        std::cout << "connected:";
    }
    bson_oid_init(&oid, nullptr);
    init_node(
            uri_string, get_oid(),
            input, output,
            "", ""
    );
    set_callback(callback);
}

Node::Node(){
    //init_node("mongodb://localhost:27017", oid_s, get_port(input_port_oid), get_port(output_port_oid), callback, callback_type);
}

Node::Node(const char *uri_string, std::string json_string_node) :
Node(uri_string){
    if(connect_to_uri(uri_string)){
        from_json(json_string_node);
    }
}

Node::Node(const char *uri_string){
    mongoc_init();
    connect_to_uri(uri_string);
    bson_oid_init (&oid, nullptr);
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


// Destructor
//--------------------------------------------------------------------
Node::~Node() {
    /*
     * Release our handles and clean up mongoc
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
    mongoc_init();

    // Database
    //----------------------------------------------------------------
    std::clog << "connecting:" << uri_string << ":";
    uri = mongoc_uri_new_with_error (uri_string, &error);
    if (!uri) {
        std::cerr << "failed to parse URI:" << uri_string << std::endl;
        std::cerr << "error message:       " << error.message << std::endl;
        return false;
    } else{
        /*
        * Create a new client instance
        */
        client = mongoc_client_new_from_uri (uri);
        /*
        * Register the application name so we can track it in the profile logs
        * on the server. This can also be done from the URI (see other examples).
        */
        mongoc_client_set_appname (client, "chinet-node");
        /*
         * Get a handle on the collections
         */
        node_collection = mongoc_client_get_collection (client, "db_chinet", "nodes");
        port_collection = mongoc_client_get_collection (client, "db_chinet", "ports");
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
    std::clog << "update:callback_type:" << callback_type;
    switch(callback_type){
        case 1: {             // C
            std::clog << ":registered C function"  << std::endl;
            rttr::method meth = rttr::type::get_global_method(callback);
            if (meth) {
                rttr::variant return_value = meth.invoke({}, input_port, output_port);
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

bool Node::write_input_port_to_db() {
    bool v = true;
    bson_t *query;
    bson_t *update = nullptr;
    bson_t reply;

    query = BCON_NEW ("_id", BCON_OID(&(input_port->oid)));
    // update by upsert, meaning if the oid is not in the DB add the document of the port
    if(!mongoc_collection_find_and_modify(port_collection, query, nullptr, input_port->document, nullptr, false, true, false, &reply, &error)){
        std::cerr << "input_port: " << error.message;
        v &= false;
    }

    // destroy
    bson_destroy(update);
    bson_destroy(query);
    bson_destroy(&reply);

    return v;
}

bool Node::write_output_port_to_db() {
    bool v = true;

    bson_t *query;
    bson_t *update = nullptr;
    bson_t reply;

    query = BCON_NEW ("_id", BCON_OID(&(output_port->oid)));
    // update by upsert, meaning if the oid is not in the DB add the document of the port
    if(!mongoc_collection_find_and_modify(port_collection, query, nullptr, output_port->document, nullptr, false, true, false, &reply, &error)){
        std::cerr << "output_port: " << error.message;
        v &= false;
    }

    // destroy
    bson_destroy(update);
    bson_destroy(query);
    bson_destroy(&reply);

    return v;
}


bool Node::write_ports_to_db(){
    // write input_port to db
    bool v = true;
    v &= write_input_port_to_db();
    v &= write_output_port_to_db();
    return v;
    /*
     if (!mongoc_collection_insert_one (node_collection, document, nullptr, nullptr, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }

    //mongoc_collection_insert_one(port_collection, input_port->document, nullptr, nullptr, nullptr);
    //mongoc_collection_insert_one(port_collection, output_port->document, nullptr, nullptr, nullptr);
    // input node
    // set the predecessor and update the oid
     */
}

bool Node::write_node_to_db(){
    // update the references to the input and output ports
    bson_t *update = nullptr;
    bson_t *query;
    bson_t reply;
    query = BCON_NEW ("_id", BCON_OID(&oid));
    update = BCON_NEW ("$set",
                       "{",
                       "input_port_oid", BCON_OID(&(input_port->oid)),
                       "output_port_oid", BCON_OID(&(output_port->oid)),
                       "callback", BCON_UTF8(callback.c_str()),
                       "callback_type", BCON_UTF8("C"),
                       "}");
    // update by upsert, meaning if the oid is not in the DB add the document of the port
    if (!mongoc_collection_find_and_modify(node_collection, query, nullptr, update, nullptr, false, true, false, &reply, &error)) {
        std::cerr << "node_collection:" << error.message << std::endl;
        return false;
    }
    //std::clog << "update reply:" << bson_as_json(&reply, nullptr) << std::endl;

    // destroy
    bson_destroy(&reply);
    bson_destroy(query);
    return true;
}

bool Node::write_to_db(){
    bool v = true;
    v &= write_ports_to_db();
    v &= write_node_to_db();
    return v;
}

bool Node::append_ports_in_db(){
    bool v = true;

    // 1. set time of death of the ports and write to DB
    input_port->set_death_time(Functions::get_time());
    output_port->set_death_time(Functions::get_time());
    v &= write_node_to_db();

    // 2. set successor
    input_port->set_birth_time(Functions::get_time());
    output_port->set_birth_time(Functions::get_time());

    input_port->set_death_time(0);
    output_port->set_death_time(0);

    input_port->set_predecessor(input_port->oid);
    output_port->set_predecessor(input_port->oid);

    input_port->new_oid();
    output_port->new_oid();

    set_input_port(input_port);
    set_output_port(output_port);

    v &= write_to_db();

    return v;
}

void Node::from_bson(const bson_t *doc){
    std::clog << "bson:" << std::endl;
    document = bson_copy(doc);

    bson_iter_t iter;
    bson_oid_t port_oid;
    char *oid_s = new char[25];

    // set oid
    if (bson_iter_init(&iter, document) &&
        bson_iter_find(&iter, "_id") &&
        BSON_ITER_HOLDS_OID(&iter)) {
        oid = *bson_iter_oid(&iter);
        bson_oid_to_string(&oid, oid_s);
        std::clog << "    node_oid: " << oid_s << std::endl;
    }

    // set input
    if (bson_iter_init(&iter, document) &&
        bson_iter_find(&iter, "input_port_oid") &&
        BSON_ITER_HOLDS_OID(&iter)) {
        port_oid = *bson_iter_oid(&iter);
        bson_oid_to_string(&port_oid, oid_s);
        std::clog << "    input_port_oid: " << oid_s << std::endl;
        input_port = get_port(&port_oid);
    }

    // set output
    if (bson_iter_init(&iter, document) &&
        bson_iter_find(&iter, "output_port_oid") &&
        BSON_ITER_HOLDS_OID(&iter)) {
        port_oid = *bson_iter_oid(&iter);
        bson_oid_to_string(&port_oid, oid_s);
        std::clog << "    output_port_oid: " << oid_s << std::endl;
        output_port = get_port(&port_oid);
    }

    // set callback
    std::string s_callback;
    std::string s_callback_type;
    if (bson_iter_init(&iter, document) &&
        bson_iter_find(&iter, "callback") &&
        BSON_ITER_HOLDS_UTF8(&iter)) {
        uint32_t l;
        const char *s = bson_iter_utf8(&iter, &l);
        s_callback = std::string(s, l);
        std::clog << "    callback: " << s_callback << std::endl;
    }
    if (bson_iter_init(&iter, document) &&
        bson_iter_find(&iter, "callback_type") &&
        BSON_ITER_HOLDS_UTF8(&iter)) {
        uint32_t l;
        const char *s = bson_iter_utf8(&iter, &l);
        s_callback_type = std::string(s, l);
        std::clog << "    callback_type: " << s_callback_type << std::endl;
    }
    set_callback(s_callback, s_callback_type);
}

void Node::from_json(const std::string &json_string){
    std::clog << "json:";
    const bson_t *doc = bson_new_from_json(
            (uint8_t*)json_string.c_str(),
            json_string.size(),
            &error
    );
    from_bson(doc);
}

void Node::from_oid(bson_oid_t *oid_doc){
    std::clog << "oid_t:";
    const bson_t *doc;
    mongoc_cursor_t *cursor;
    bson_t *query;

    query = BCON_NEW ("_id", BCON_OID(oid_doc));
    cursor = mongoc_collection_find_with_opts(node_collection, query, nullptr, nullptr);
    while (mongoc_cursor_next (cursor, &doc)) {
        from_bson(doc);
    }
    mongoc_cursor_destroy(cursor);
    bson_destroy(query);
}

void Node::from_oid(const std::string oid_doc){
    std::clog << "oid_s:";
    bson_oid_t oid_d;
    bson_oid_init_from_string(&oid_d, oid_doc.c_str());
    from_oid(&oid_d);
}

std::string Node::to_json(){
    size_t len;
    char* str = bson_as_json(document, &len);
    return std::string(str, len);
}

// Getter
//--------------------------------------------------------------------

const bson_t* Node::get_document(){
    /*
    bson_t *query;
    const bson_t *doc;
    query = BCON_NEW ("_id", BCON_OID(&(oid)));
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(node_collection, query, NULL, NULL);
    mongoc_cursor_next (cursor, &doc);
    return doc;
     */
    return document;
}

std::string Node::get_name(){
    if(input_port != nullptr){
        std::string r;
        r.append(callback);
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

/*
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
    if (bson_iter_init(&iter, document) && bson_iter_find(&iter, "input_port_oid")) {
        if (bson_iter_init (&iter, document) && BSON_ITER_HOLDS_OID(&iter)) {
            uint32_t l = 25;
            char *oid_s = new char[l];
            bson_oid_t const *s  = bson_iter_oid(&iter);
            bson_oid_to_string(s, oid_s);
            return std::string(oid_s, l);
        }
    }
    return "";
}
 */

// Setter
//--------------------------------------------------------------------
void Node::set_input_port(std::shared_ptr<Port> input) {
    input_port = input;
    input_port_oid = input->oid;

    bson_iter_t iter;
    if (bson_iter_init (&iter, document) &&
        bson_iter_find (&iter, "input_port_oid") &&
        BSON_ITER_HOLDS_OID (&iter)) {
        bson_iter_overwrite_oid(&iter, &(input->oid));
        std::clog << "new input_port_oid: " << input->get_oid() << std::endl;
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
    output_port_oid = output->oid;

    bson_iter_t iter;
    if (bson_iter_init (&iter, document) &&
        bson_iter_find (&iter, "output_port_oid") &&
        BSON_ITER_HOLDS_OID (&iter)) {
        bson_iter_overwrite_oid(&iter, &(output->oid));
        std::clog << "new output_port_oid: " << output->get_oid() << std::endl;
    }
}

void Node::set_callback(std::string &s_callback, std::string &s_callback_type){
    this->callback = s_callback;
    this->callback_type_s = s_callback_type;
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

// Private
//--------------------------------------------------------------------

// Methods
//--------------------------------------------------------------------
std::shared_ptr<Port> Node::get_port(bson_oid_t *oid_port){
    bson_t * query =  BCON_NEW ("_id", BCON_OID(oid_port));
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(port_collection, query, nullptr, nullptr);

    // read the data
    const bson_t *doc;
    auto p = std::make_shared<Port>(*oid_port);

    int n = 0;
    while (mongoc_cursor_next (cursor, &doc)) {
        p->document = bson_copy(doc);
        n++;
        break;
    }
    if(n<1){
        std::cerr << "get_port:port not in DB:";
    }
    if (mongoc_cursor_error (cursor, &error)) {
        std::cerr << "get_port:" << error.message;
    }

    // destroy
    mongoc_cursor_destroy(cursor);
    bson_destroy(query);

    std::cerr << std::endl;

    return p;
}

std::shared_ptr<Port> Node::get_port(const char* oid_string){
    // convert the string to an bson_oid_t
    bson_oid_t oid_port;
    bson_oid_init_from_string(&oid_port, oid_string);
    return get_port(&oid_port);
}

void Node::init_node(
        const char *uri_string,
        std::string oid_s,
        std::shared_ptr<Port> input,
        std::shared_ptr<Port> output,
        std::string callback,
        std::string call_back_type
){

    // initialize oids
    std::clog << "oid:" << oid_s << ":";
    bson_oid_init_from_string (&oid, oid_s.c_str());
    bson_oid_copy(&oid, &input_port_oid);
    bson_oid_copy(&oid, &output_port_oid);

    set_callback(callback, call_back_type);
    document = BCON_NEW(
            "_id", BCON_OID(&oid),
            "input_port_oid", BCON_OID(&input_port_oid),
            "output_port_oid", BCON_OID(&output_port_oid),
            "callback", BCON_UTF8(callback.c_str()),
            "callback_type", BCON_UTF8(callback_type_s.c_str()),
            "birth", BCON_INT64(Functions::get_time()),
            "death", BCON_INT64(0)
    );
    if (!mongoc_collection_insert_one (node_collection, document, nullptr, nullptr, &error)) {
        std::cerr << "Node already exists" << error.message << std::endl;
    }

    set_input_port(input);
    set_output_port(output);
}