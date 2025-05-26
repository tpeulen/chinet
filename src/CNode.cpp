#include "CNode.h"
#include "Port.h"
#include "info.h"

// Constructor
//--------------------------------------------------------------------

Node::Node(
        std::string name,
        const std::map<std::string, std::shared_ptr<Port>>& ports,
        std::shared_ptr<NodeCallback> callback_class
) : DatabaseObject(name)
{
#ifdef WITH_MONGODB
    append_string(&document, "type", "node");
#else
    document["type"] = "node";
#endif
    set_ports(ports);
    if(callback_class != nullptr){
        this->callback_class = callback_class;
    }
}


// Destructor
//--------------------------------------------------------------------
Node::~Node() = default;


// Methods
//--------------------------------------------------------------------

bool Node::read_from_db(const std::string &oid_string){
    if (is_chinet_verbose()) {
        std::clog << "READING NODE FROM DB" << std::endl;
        std::clog << "Requested OID:" << oid_string << std::endl;
    }
    bool return_value = true;
    return_value &= DatabaseObject::read_from_db(oid_string);

#ifdef WITH_MONGODB
    return_value &= create_and_connect_objects_from_oid_doc(
            &document, "ports", &ports
            );
    if (is_chinet_verbose()) {
        std::clog << "callback-restore: " << get_string_by_key(&document, "callback") << std::endl;
        std::clog << "callback_type-restore: " << get_string_by_key(&document, "callback_type") << std::endl;
    }

    set_callback(
            get_string_by_key(&document, "callback"),
            get_string_by_key(&document, "callback_type")
            );
#else
    return_value &= create_and_connect_objects_from_oid_doc(
            document, "ports", &ports
            );
    if (is_chinet_verbose()) {
        std::clog << "callback-restore: " << document["callback"].get<std::string>() << std::endl;
        std::clog << "callback_type-restore: " << document["callback_type"].get<std::string>() << std::endl;
    }

    set_callback(
            document["callback"].get<std::string>(),
            document["callback_type"].get<std::string>()
            );
#endif

    return return_value;
}

bool Node::write_to_db() {
    bool re = DatabaseObject::write_to_db();

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
    r.append(":");
    r.append(callback);
    r.append(":");
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

void Node::set_ports(const std::map<std::string, std::shared_ptr<Port>>& ports){
    for(auto &o: ports){
        o.second->set_name(o.first);
        add_port(o.first, o.second, o.second->is_output(), false);
    }
    fill_input_output_port_lookups();
}

Port* Node::get_port(const std::string &port_name){
    return ports[port_name].get();
}

Port* Node::get_input_port(const std::string &port_name){
    return in_[port_name].get();
}

Port* Node::get_output_port(const std::string &port_name){
    return out_[port_name].get();
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
    if (is_chinet_verbose()) {
        std::clog << "NODE SET CALLBACK" << std::endl;
    }
    this->callback = s_callback;
    this->callback_type_string = s_callback_type;
    if (is_chinet_verbose()) {
        std::clog << "-- Callback type: " << callback_type_string << std::endl;
        std::clog << "-- Callback name: " << callback << std::endl;
    }
    if(s_callback_type == "C"){
        callback_type = 0;
        meth_ = rttr::type::get_global_method(callback);
        if(!meth_){
            if (is_chinet_verbose()) {
                std::cerr << "ERROR: The class type " << callback << " does not exist." <<
                          " No callback set. " << std::endl;
            }
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
void Node::add_port(
        const std::string &key,
        std::shared_ptr<Port> port,
        bool is_output,
        bool fill_in_out
        ) {
    if (is_chinet_verbose()) {
        std::clog << "ADDING PORT TO NODE" << std::endl;
        std::clog << "-- Name of node: " << get_name() << std::endl;
        std::clog << "-- Key of port: " << key << std::endl;
        std::clog << "-- Port is_output: " << is_output << std::endl;
        std::clog << "-- Fill value of output: " << fill_in_out << std::endl;
    }
    port->set_port_type(is_output);
    port->set_node(this);
    if (ports.find(key) == ports.end() ) {
        if (is_chinet_verbose()) {
            std::clog << "-- The key of the port was not found." << std::endl;
            std::clog << "-- Port " << key << " was created in node. " << std::endl;
        }
        ports[key] = port;
    } else {
        auto p = ports[key];
        if(port != p){
            if (is_chinet_verbose()) {
                std::clog << "WARNING: Overwriting the port that was originally associated to the key " << key << "." << std::endl;
            }
            ports[key] = port;
        } else{
            std::cerr << "WARNING: Port is already part of the node." << std::endl;
            std::cerr << "-- Assigning Port to the key: " << key << "." << std::endl;
        }
    }
    if(fill_in_out){
        fill_input_output_port_lookups();
    }
}

void Node::add_input_port(
        const std::string &key,
        std::shared_ptr<Port> port
        ) {
    add_port(key, port, false);
}

void Node::add_output_port(
        const std::string &key,
        std::shared_ptr<Port> port
        ) {
    add_port(key, port, true);
}

#ifdef WITH_MONGODB
bson_t Node::get_bson(){
    // Since we're inheriting from DatabaseObject which is a typedef for MongoObject when WITH_MONGODB is defined,
    // we can safely cast to MongoObject* here
    bson_t dst = static_cast<MongoObject*>(this)->get_bson_excluding(
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
#endif

void Node::evaluate(){
    if (is_chinet_verbose()) {
        std::clog << "NODE EVALUATE" << std::endl;
        std::clog << "-- Node name: " << get_name() << std::endl;
        std::clog << "-- Callback_type: " << callback_type << std::endl;
    }
    if(callback_type == 0)
    {
        if (is_chinet_verbose()) {
            std::clog << "-- Calling registered C function."  << std::endl;
        }
        rttr::variant return_value = meth_.invoke({}, in_, out_);
    } else if (callback_class != nullptr) {
        if (is_chinet_verbose()) {
            std::clog << "-- Calling 'run' method of a callback class."  << std::endl;
        }
            callback_class->run(in_, out_);
    }
    if (is_chinet_verbose()) {
        std::clog << "-- Setting nodes associated to output ports to invalid."  << std::endl;
    }
    for(auto &o : get_output_ports()){
        auto n = o.second->get_node();
        if(n != nullptr){
            if (is_chinet_verbose()) {
                std::clog << "-- Node " << n->get_name() << " of port " << o.second->get_name() << " set to invalid." << std::endl;
            }
            n->set_valid(false);
        }
    }
    node_valid_ = true;
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
        if(input_port->is_linked()){
            auto output_port = input_port->get_link();
            auto output_node = output_port->get_node();
            if(output_node == this) return true;
            else if(!output_node->is_valid()) return false;
        }
    }
    return true;
}

void Node::set_valid(bool is_valid){
    node_valid_ = is_valid;
    for(auto &v : out_)
    {
        auto output_port = v.second;
        //v.second->set_invalid();
    }
}

bool Node::is_valid(){
    if(get_input_ports().empty()) return true;
    else if(!inputs_valid()) return false;
    else return node_valid_;
}
