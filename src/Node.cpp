//#include <Port.h>
#include "Node.h"

// Constructor
//--------------------------------------------------------------------


Node::Node(){
    append_string(&document, "type", "node");
}

Node::Node(std::string name) :
MongoObject(name)
{
    append_string(&document, "type", "node");
}

Node::Node(std::map<std::string, std::shared_ptr<Port>> ports) :
           Node(){
    this->ports = ports;
    for(auto &o: ports){
        o.second->set_name(o.first);
    }
    fill_input_output_port_lookups();
}

/*
Node::Node(std::map<std::string, double> input_ports,
           std::map<std::string, double> output_ports) :
        Node(){
    for(auto o : input_ports){
        auto p = std::make_shared<Port>(o.second);
        add_input_port(o.first, p);
    }
    for(auto o : output_ports){
        auto p = std::make_shared<Port>(o.second);
        add_output_port(o.first, p);
    }
}
 */

// Destructor
//--------------------------------------------------------------------
Node::~Node() {
}


// Methods
//--------------------------------------------------------------------

bool Node::read_from_db(const std::string &oid_string){
    bool return_value = true;

    return_value &= MongoObject::read_from_db(oid_string);
    return_value &= create_and_connect_objects_from_oid_doc(&document, "ports", &ports);
#if CHINET_DEBUG
    std::cout << "callback-restore: " << get_string_by_key(&document, "callback") << std::endl;
    std::cout << "callback_type-restore: " << get_string_by_key(&document, "callback_type") << std::endl;
#endif

    set_callback(
            get_string_by_key(&document, "callback"),
            get_string_by_key(&document, "callback_type")
            );

    return return_value;
}

bool Node::write_to_db() {
    bool re = MongoObject::write_to_db();

    for(auto &o : ports){
        if(!o.second->is_connected_to_db()){
            re &= connect_object_to_db(o.second);
        }
        o.second->write_to_db();
    }

    return re;
}

// Getter
//--------------------------------------------------------------------

std::string Node::get_name(){
    std::string r;
    r.append(object_name);
    r.append(", ");
    r.append(callback);
    r.append(": ");
    r.append("(");
    for(auto const &n : get_input_ports()){
        r.append(n.first);
        r.append(",");
    }
    r.append(")");

    r.append("->");

    r.append("(");
    for(auto const &n : get_output_ports()){
        r.append(n.first);
        r.append(",");
    }
    r.append(")");

    return r;
}

std::map<std::string, std::shared_ptr<Port>> Node::get_ports(){
    return ports;
}

std::shared_ptr<Port> Node::get_port(const std::string &port_name){
    return ports[port_name];
}

std::shared_ptr<Port> Node::get_input_port(const std::string &port_name){
    return in_[port_name];
}

std::shared_ptr<Port> Node::get_output_port(const std::string &port_name){
    return out_[port_name];
}

std::map<std::string, std::shared_ptr<Port>> Node::get_input_ports(){
    return in_;
}

std::map<std::string, std::shared_ptr<Port>> Node::get_output_ports(){
    return out_;
}

// Setter
//--------------------------------------------------------------------
void Node::set_callback(std::string s_callback, std::string s_callback_type){
    this->callback = s_callback;
    this->callback_type_string = s_callback_type;
    if(s_callback_type == "C"){
        callback_type = 0;
        meth_ = rttr::type::get_global_method(callback);
        if(!meth_){
#if CHINET_DEBUG
            std::cerr << "The class type " << callback << " does not exist." <<
                      " No callback set. " << std::endl;
#endif
        }
    }
}

void Node::set_callback(std::shared_ptr<NodeCallback> cb){
    callback_class = cb;
    callback_type = 1;
}

// Private
//--------------------------------------------------------------------

// Methods
//--------------------------------------------------------------------
void Node::add_port(std::string key, std::shared_ptr<Port> port, bool is_output) {
    port->set_port_type(is_output);
    ports[key] = port;
    fill_input_output_port_lookups();
}

void Node::add_input_port(std::string key, std::shared_ptr<Port> port) {
    add_port(key, port, false);
    fill_input_output_port_lookups();
}

void Node::add_output_port(std::string key, std::shared_ptr<Port> port) {
    add_port(key, port, true);
    fill_input_output_port_lookups();
}

bson_t Node::get_bson(){
    bson_t dst = MongoObject::get_bson_excluding(
            "input_ports",
            "output_ports",
             "callback",
             "callback_type",
             NULL
    );

    create_oid_dict_in_doc<Port>(&dst, "ports", ports);

    append_string(&dst, "callback", callback);
    append_string(&dst, "callback_type", callback_type_string);

    return dst;
}

void Node::evaluate(){
    #if CHINET_DEBUG
    std::clog << "update:callback_type:" << callback_type;
    #endif
    if(callback_type == 0)
    {
        #if CHINET_DEBUG
        std::clog << ":registered C function"  << std::endl;
        #endif
        rttr::variant return_value = meth_.invoke({}, in_, out_);
    } else if (callback_class != nullptr) {
            callback_class->run(in_, out_);
    }
}

bool Node::is_valid(){
    if(get_input_ports().empty()){
        return true;
    } else{
        return node_valid_;
    }
}

void Node::fill_input_output_port_lookups(){
    out_.clear();
    in_.clear();

    for(auto o: ports){
        if(o.second->is_output()){
            out_[o.first] = o.second;
        } else{
            in_[o.first] = o.second;
        }
    }
}


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

