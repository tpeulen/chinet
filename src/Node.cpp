//#include <Port.h>
#include "Node.h"
#include "Port.h"

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
    for(auto &o: ports){
        o.second->set_name(o.first);
        add_port(o.first, o.second, o.second->is_output(), false);
    }
    fill_input_output_port_lookups();
}

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
void Node::add_port(std::string key,
        std::shared_ptr<Port> port,
        bool is_output,
        bool fill_in_out) {
    port->set_port_type(is_output);
    port->set_node(this);
    ports[key] = port;
    if(fill_in_out){
        fill_input_output_port_lookups();
    }
}

void Node::add_input_port(std::string key, std::shared_ptr<Port> port) {
    add_port(key, port, false);
}

void Node::add_output_port(std::string key, std::shared_ptr<Port> port) {
    add_port(key, port, true);
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
    for(auto &o : get_output_ports()){
        auto n = o.second->get_node();
        if(n != nullptr){
            n->set_node_to_invalid();
        }
    }
    node_valid_ = true;
}

bool Node::is_valid(){
    if(get_input_ports().empty()){
        return true;
    }else if(!inputs_valid()){
        return false;
    }else{
        return node_valid_;
    }
}

void Node::fill_input_output_port_lookups(){
    out_.clear();
    in_.clear();

    for(auto &o: ports){
        if(o.second->is_output()){
            out_[o.first] = o.second;
        } else{
            in_[o.first] = o.second;
        }
    }
}

bool Node::inputs_valid(){
    for(const auto &i : in_){
        auto input_port = i.second;
        if(input_port->port_links.is_linked()){
            auto output_port = input_port->port_links.get_link();
            auto output_node = output_port->get_node();
            if(!output_node->is_valid())
            {
                return false;
            }
        }
    }
    return true;
}

void Node::set_node_to_invalid(){
    node_valid_ = false;
    for(auto &v : out_)
    {
        auto output_port = v.second;
        //v.second->set_invalid();
    }
}
