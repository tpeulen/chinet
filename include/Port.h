#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <memory>
#include <string>
#include <string_view>
#include <cmath>
#include <chrono>
#include "bson.h"

#include <Functions.h>
#include <Node.h>
#include <Value.h>
#include <Link.h>


class Node;
class Value;
class Link;

class Port : public std::enable_shared_from_this<Port>{

    friend Node;
    friend Value;
    friend Link;

private:

    std::map<std::string, std::tuple<std::shared_ptr<Value>, std::shared_ptr<Link>>> _values;
    Node *node;

    //std::shared_ptr<Node> node;
    std::shared_ptr<Port> input;

    /// stores the data of the Port in a BSON document
    bson_t *document;

    /// the object identifier (unique number)
    std::vector<std::string> skip = {"_id", "predecessor", "birth", "death"};
    bson_oid_t oid;

    bson_oid_t predecessor;
    uint64_t birth;
    uint64_t death;

public:

    bool set_v(std::string key, std::shared_ptr<Value> v);
    bool set_v(std::string key, std::shared_ptr<Value> v, std::shared_ptr<Link> l);
    std::shared_ptr<Value> get_v(std::string key);
    //std::shared_ptr<Link> link(std::string key, Port *target_port, std::string target_key);

    // Constructor
    //--------------------------------------------------------------------
    Port();
    Port(std::string json_string);
    Port(bson_oid_t oid);
    Port(const char *uri, bson_oid_t oid);
    //Port(std::shared_ptr<Node> node);
    //Port(std::string json_string, std::shared_ptr<Node> node);

    // Destructor
    //--------------------------------------------------------------------
    ~Port();


    // Getter
    //--------------------------------------------------------------------
    bson_t* get_value();
    std::string to_json();
    //std::string get_name();
    std::shared_ptr<Port> get_input();
    /// returns the value of a slot for a given key (slot name)
    std::vector<double> value(const std::string &slot_key);
    /// returns the values of all the slots as an vector
    std::map<std::string, std::vector<double>> values();
    /// returns the keys of all the slots as an vector
    std::vector<std::string> keys();
    std::shared_ptr<Port> shared_ptr();
    std::string get_oid();

    // Setter
    //--------------------------------------------------------------------
    void set_input(std::shared_ptr<Port> v);
    bool set_slot_value(std::string slot_key, std::vector<double> values);
    void set_oid(std::string v);
    //void set_node(std::shared_ptr<Node> node);
    bool set_predecessor(bson_oid_t v);
    bool set_death_time(uint64_t v);
    bool set_birth_time(uint64_t v);

    // Operator
    //--------------------------------------------------------------------
    std::shared_ptr<Port> operator +(std::shared_ptr<Port> v);
    std::vector<double> operator [](std::string key);

    // Methods
    //--------------------------------------------------------------------
    void from_json(const std::string &json_string);
    //bool add_port_to_collection(mongoc_collection_t * port_collection);
    void new_oid();
    virtual size_t size();
    bson_t *to_bson();

};


#endif //chinet_PORT_H
