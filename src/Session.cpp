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
        if (it_val.key() == "fixed") {
            port->set_fixed(
                    j["nodes"][node_key]["ports"][port_key]["fixed"].get<bool>()
                    );
        } else if (it_val.key() == "value") {
            auto b = j["nodes"][node_key]["ports"][port_key]["value"].get<std::vector<double>>();
            port->set_value(b.data(), b.size());
        } else if (it_val.key() == "is_output"){
            port->set_port_type(
                    j["nodes"][node_key]["ports"][port_key]["is_output"].get<bool>()
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
        std::string node_key = it.key();
        add_node(node_key, read_node_template(j, node_key));
    }
    return true;
}

