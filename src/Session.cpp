//
// Created by thomas on 4/23/19.
//

#include "Session.h"

bool Session::read_from_db(const std::string &oid_string){
    bool return_value = true;
    return_value &= MongoObject::read_from_db(oid_string);
    return_value &= create_and_connect_objects_from_oid_array(&document, "nodes", &nodes);
    return return_value;
}

bool Session::write_to_db(){

    bool re = MongoObject::write_to_db();

    for(auto &o : nodes){
        if(!o.second->is_connected_to_db()){
            re &= connect_object_to_db(o.second);
        }
        o.second->write_to_db();
    }

    return re;
}

bson_t Session::get_bson(){
    bson_t doc = MongoObject::get_bson_excluding("nodes", NULL);
    create_oid_array_in_doc(&doc, "nodes", nodes);
    return doc;
}

std::shared_ptr<Port> Session::read_port_template(json j, std::string &node_key, std::string &port_key) {
    auto port = std::make_shared<Port>();

    auto port_ = j["nodes"][node_key]["ports"][port_key];
    for (json::iterator it_val = port_.begin(); it_val != port_.end(); ++it_val) {
        if (it_val.key() == "is_fixed") {
            port->set_fixed(
                    j["nodes"][node_key]["ports"][port_key]["is_fixed"].get<bool>()
                    );
        } else if (it_val.key() == "value") {
            auto b = j["nodes"][node_key]["ports"][port_key]["value"].get<std::vector<double>>();
            port->set_value(b.data(), b.size());
        } else if (it_val.key() == "is_output"){
            port->set_port_type(
                    j["nodes"][node_key]["ports"][port_key]["is_output"].get<bool>()
            );
        } else if (it_val.key() == "is_reactive"){
            port->set_reactive(
                    j["nodes"][node_key]["ports"][port_key]["is_reactive"].get<bool>()
            );
        }
    }
    port->set_name(port_key);
    return port;
}

std::shared_ptr<Node> Session::read_node_template(json j, std::string &node_key){
    auto node = std::make_shared<Node>(node_key);

    auto inputs_ = j["nodes"][node_key]["ports"];
    for (json::iterator it = inputs_.begin(); it != inputs_.end(); ++it) {
        std::string port_key = it.key();
        auto port = read_port_template(j, node_key, port_key);
        node->add_port(port_key, port, port->is_output());
    }
    node->set_callback(
            j["nodes"][node_key]["callback"].get<std::string>(),
            j["nodes"][node_key]["callback_type"].get<std::string>()
            );

    return node;
}

bool Session::read_session_template(const std::string &json_string){
    json j = json::parse(json_string);

    // read nodes
    auto n = j["nodes"];
    for (json::iterator it = n.begin(); it != n.end(); ++it) {
        auto node_key = it.key();
        add_node(node_key, read_node_template(j, node_key));
    }

    auto l = j["links"];
    for (json::iterator it = l.begin(); it != l.end(); ++it) {
        auto v = it.value();
        link_nodes(
                v["node"].get<std::string>(),
                v["port"].get<std::string>(),
                v["target_node"].get<std::string>(),
                v["target_port"].get<std::string>()
                );
    }

    return true;
}

bool Session::link_nodes(
        const std::string node_name,
        const std::string port_name,
        const std::string target_node_name,
        const std::string target_port_name){
    auto itn = nodes.find(node_name);
    auto itnt = nodes.find(target_node_name);

    if(itn != nodes.end() && itnt != nodes.end()){
        auto ports = nodes[node_name]->get_ports();
        auto target_ports = nodes[target_node_name]->get_ports();
        auto itp = ports.find(port_name);
        auto itpt = ports.find(target_port_name);
        if(itp != ports.end() && itpt != target_ports.end()){
            ports[port_name]->link(target_ports[target_port_name]);
            return true;
        }
    }
    return false;
}


