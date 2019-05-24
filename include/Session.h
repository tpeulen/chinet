//
// Created by thomas on 4/23/19.
//

#ifndef CHINET_SESSION_H
#define CHINET_SESSION_H

#include <map>
#include <memory>
#include <nlohmann/json.hpp>

#include "MongoObject.h"
#include "Node.h"
#include "Port.h"


using nlohmann::json;

class Session : public MongoObject
{

protected:

    bson_t get_bson() final;

public:

    std::map<std::string, std::shared_ptr<Node>> nodes;

    Session()
    {
        append_string(&document, "type", "session");
    };

    Session(std::map<std::string, std::shared_ptr<Node>> nodes) :
            Session()
    {
        for (auto &o : nodes) {
            add_node(o.first, o.second);
        }
    };

    void add_node(std::string name, std::shared_ptr<Node> object);

    std::map<std::string, std::shared_ptr<Node>> get_nodes();

    bool write_to_db() final;

    bool read_from_db(const std::string &oid_string) final;

    std::shared_ptr<Port> create_port(json port_template, std::string &port_key);

    std::shared_ptr<Node> create_node(json node_template, std::string &node_key);

    bool read_session_template(const std::string &json_string);

    std::string get_session_template();

    bool link_nodes(
            const std::string &node_name,
            const std::string &port_name,
            const std::string &target_node_name,
            const std::string &target_port_name);

};


#endif //CHINET_SESSION_H
