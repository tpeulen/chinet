//#include <Port.h>
#include "Node.h"

// Constructor
//--------------------------------------------------------------------


Node::Node(){
    bson_append_utf8(&document, "type", 4, "node", 4);
}


// Destructor
//--------------------------------------------------------------------
Node::~Node() {
}


// Methods
//--------------------------------------------------------------------

void Node::set_callback(std::shared_ptr<NodeCallback> cb){
    callback_class = cb;
}

bool Node::read_from_db(const std::string &oid_string){
    bool return_value = MongoObject::read_from_db(oid_string);
    create_and_connect_objects_from_oids(&document, "input_ports", &input_ports);
    return return_value;
}

// Getter
//--------------------------------------------------------------------


std::string Node::get_name(){
    std::string r;
    r.append(callback);

    r.append("(");
    for(auto const &n : input_ports){
        r.append(n.first);
        r.append(",");
    }
    r.append(")");

    r.append("->");

    r.append("(");
    for(auto const &n : output_ports){
        r.append(n.first);
        r.append(",");
    }
    r.append(")");

    return r;
}

std::vector<std::string> Node::get_input_port_oids(){
    std::vector<std::string> re;
    for(auto const &v : input_ports){
        re.emplace_back(v.second->get_oid_string());
    }
    return re;
}

std::vector<std::string> Node::get_output_port_oids(){
    std::vector<std::string> re;
    for(auto const &v : output_ports){
        re.emplace_back(v.second->get_oid_string());
    }
    return re;
}

std::vector<std::string> Node::get_port_oids(){
    std::vector<std::string> re;
    for(auto const &v : get_input_port_oids()){
        re.push_back(v);
    }
    for(auto const &v : get_output_port_oids()){
        re.push_back(v);
    }
    return re;
}


// Setter
//--------------------------------------------------------------------


void Node::set_callback(std::string &s_callback, std::string &s_callback_type){
    this->callback = s_callback;
    this->callback_type_str = s_callback_type;
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

void Node::add_input_port(std::string key, std::shared_ptr<Port> port) {
    input_ports[key] = port;
}

void Node::add_output_port(std::string key, std::shared_ptr<Port> port) {
    output_ports[key] = port;
}

bson_t Node::get_bson(){

    bson_t src = MongoObject::get_bson();
    bson_t dst; bson_init (&dst);
    bson_copy_to_excluding_noinit(&src, &dst,
            "input_ports",
            "output_ports",
            NULL
    );

    create_oid_dict_in_doc(&dst, "input_ports", input_ports);
    create_oid_dict_in_doc(&dst, "output_ports", output_ports);

    return dst;
}


/*
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
 */


/*
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

 */

/*
std::shared_ptr<Port> Node::get_output_port(){
    return this->output_port;
}

std::shared_ptr<Port> Node::get_input_port(){
    return this->input_port;
}
 */


/*

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

/*
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
 */


/*
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
 */

/*
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

 */

/*
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
 */

/*

std::vector<bson_oid_t> Node::get_input_port_oids(){
    bson_iter_t iter;
    if (bson_iter_init (&iter, document) &&
        bson_iter_find (&iter, "input_port_oids") &&
        BSON_ITER_HOLDS_OID (&iter))
    {
        bson_iter_overwrite_oid(&iter, &(output->oid));
        std::clog << "new output_port_oid: " << output->get_oid() << std::endl;
    }

}

*/
