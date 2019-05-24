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

std::shared_ptr<Port> Session::create_port(json port_template, std::string &port_key) {
    auto port = std::make_shared<Port>();
    port->set_name(port_key);

    for (json::iterator it_val = port_template.begin(); it_val != port_template.end(); ++it_val) {
        if (it_val.key() == "is_fixed") {
            port->set_fixed(
                    port_template["is_fixed"].get<bool>()
                    );
        } else if (it_val.key() == "value") {
            auto b = port_template["value"].get<std::vector<double>>();
            port->set_value(b.data(), b.size());
        } else if (it_val.key() == "is_output"){
            port->set_port_type(
                    port_template["is_output"].get<bool>()
            );
        } else if (it_val.key() == "is_reactive"){
            port->set_reactive(
                    port_template["is_reactive"].get<bool>()
            );
        }
    }
    return port;
}

std::shared_ptr<Node> Session::create_node(json node_template, std::string &node_key){
    auto node = std::make_shared<Node>(node_key);

    auto ports_json = node_template["ports"];
    for (json::iterator it = ports_json.begin(); it != ports_json.end(); ++it) {
        std::string port_key = it.key();

        auto port_json = node_template["ports"][port_key];
        auto port = create_port(port_json, port_key);

        node->add_port(port_key, port, port->is_output());
    }

    node->set_callback(
            node_template["callback"].get<std::string>(),
            node_template["callback_type"].get<std::string>()
            );

    return node;
}

bool Session::read_session_template(const std::string &json_string){
    json session_json = json::parse(json_string);

    // read nodes
    json nodes_json = session_json["nodes"];
    for (json::iterator it = nodes_json.begin(); it != nodes_json.end(); ++it) {
        auto node_key = it.key();
        add_node(node_key, create_node(nodes_json[node_key], node_key));
    }

    auto l = session_json["links"];
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
        const std::string &node_name,
        const std::string &port_name,
        const std::string &target_node_name,
        const std::string &target_port_name){
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

void Session::add_node(std::string name, std::shared_ptr<Node> object)
{
    nodes[name] = object;
    object->set_name(name);
    if (is_connected_to_db()) {
        connect_object_to_db(object);
    }
}

std::map<std::string, std::shared_ptr<Node>> Session::get_nodes()
{
    return nodes;
}

std::string Session::get_session_template(){

}

