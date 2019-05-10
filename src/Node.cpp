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

Node::Node(std::map<std::string, std::shared_ptr<Port>> input_ports,
           std::map<std::string, std::shared_ptr<Port>> output_ports) :
           Node(){
    this->input_ports = input_ports;
    this->output_ports = output_ports;
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
    return_value &= create_and_connect_objects_from_oid_doc(&document, "input_ports", &input_ports);
    return_value &= create_and_connect_objects_from_oid_doc(&document, "output_ports", &output_ports);

    std::cout << "callback-restore: " << get_string_by_key(&document, "callback") << std::endl;
    std::cout << "callback_type-restore: " << get_string_by_key(&document, "callback_type") << std::endl;

    set_callback(
            get_string_by_key(&document, "callback"),
            get_string_by_key(&document, "callback_type")
            );

    return return_value;
}

bool Node::write_to_db() {
    bool re = MongoObject::write_to_db();

    for(auto o : input_ports){
        if(!o.second->is_connected_to_db()){
            re &= connect_object_to_db(o.second);
        }
        o.second->write_to_db();
    }

    for(auto o : output_ports){
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

std::shared_ptr<Port> Node::get_input_port(const std::string &port_name){
    return input_ports[port_name];
}

std::shared_ptr<Port> Node::get_output_port(const std::string &port_name){
    return output_ports[port_name];
}

std::vector<std::string> Node::get_input_port_oids(){
    std::vector<std::string> re;
    for(auto const &v : input_ports){
        re.emplace_back(v.second->get_oid());
    }
    return re;
}

std::vector<std::string> Node::get_output_port_oids(){
    std::vector<std::string> re;
    for(auto const &v : output_ports){
        re.emplace_back(v.second->get_oid());
    }
    return re;
}

std::map<std::string, std::shared_ptr<Port>> Node::get_input_ports(){
    return input_ports;
}

std::map<std::string, std::shared_ptr<Port>> Node::get_output_ports(){
    return output_ports;
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
void Node::set_callback(const std::string s_callback, const std::string s_callback_type){
    this->callback = s_callback;
    this->callback_type = s_callback_type;
    if(s_callback_type == "C"){
        rttr::method meth = rttr::type::get_global_method(callback);
        if(!meth){
            std::cerr << "The class type " << callback << " does not exist." <<
                      " No callback set. " << std::endl;
        }
    }
}

void Node::set_callback(std::shared_ptr<NodeCallback> cb){
    callback_class = cb;
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
    bson_t dst = MongoObject::get_bson_excluding(
            "input_ports",
            "output_ports",
             "callback",
             "callback_type",
             NULL
    );

    create_oid_dict_in_doc<Port>(&dst, "input_ports", input_ports);
    create_oid_dict_in_doc<Port>(&dst, "output_ports", output_ports);

    append_string(&dst, "callback", callback);
    append_string(&dst, "callback_type", callback_type);

    return dst;
}

void Node::evaluate(){
    if(input_ports.empty() || output_ports.empty()) {
        return;
    }
    std::clog << "update:callback_type:" << callback_type;
    if(callback_type == "C")
    {
        std::clog << ":registered C function"  << std::endl;
        rttr::method meth = rttr::type::get_global_method(callback);
        if (meth) {
            rttr::variant return_value = meth.invoke({}, input_ports, output_ports);
        }
    } else if (callback_class != nullptr) {
            callback_class->run(input_ports, output_ports);
    }
}

bool Node::is_valid(){
    if(input_ports.empty()){
        return true;
    } else{
        return node_valid_;
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

