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

#include <MongoObject.h>
#include <Functions.h>
#include <Node.h>
#include <Link.h>

class Link;

class Port : public MongoObject{

private:
    std::shared_ptr<Link> link;

public:

    double get_value_double();
    int get_value_int();
    bool is_fixed();
    void set_value(double v);
    void set_link(std::shared_ptr<Link> v);

    // Constructor
    //--------------------------------------------------------------------
    Port()
    {
        bson_append_utf8(&document, "type", 4, "port", 4);
        bson_append_double(&document, "value", 5, 1.0);
        bson_append_bool(&document, "fixed", 5, false);
        bson_append_oid(&document, "link", 4, &oid_document);
    };

    // Destructor
    //--------------------------------------------------------------------
    ~Port() = default;

    // Getter
    //--------------------------------------------------------------------
    //bson_t* get_value();
    //std::string get_name();
    //std::shared_ptr<Port> get_input();
    /*
    /// returns the value of a slot for a given key (slot name)
    std::vector<double> value(const std::string &slot_key);
    /// returns the values of all the slots as an vector
    std::map<std::string, std::vector<double>> values();
    /// returns the keys of all the slots as an vector
    std::vector<std::string> keys();
     */
    //std::shared_ptr<Port> shared_ptr();
    //std::string get_oid();

    // Setter
    //--------------------------------------------------------------------
    //void set_input(std::shared_ptr<Port> v);
    //bool set_slot_value(std::string slot_key, std::vector<double> values);
    //void set_oid(std::string v);
    //void set_node(std::shared_ptr<Node> node);
    /*
    bool set_predecessor(bson_oid_t v);
    bool set_death_time(uint64_t v);
    bool set_birth_time(uint64_t v);
     */

    // Operator
    //--------------------------------------------------------------------
    //std::shared_ptr<Port> operator +(std::shared_ptr<Port> v);
    // std::vector<double> operator [](std::string key);

    // Methods
    //--------------------------------------------------------------------
    //void from_json(const std::string &json_string);
    //bool add_port_to_collection(mongoc_collection_t * port_collection);
    //void new_oid();
    //virtual size_t size();
    // bson_t *to_bson();

    //std::shared_ptr<Node> node;

    /*
    T get_v(std::string key);

    bool set_v(std::string key, T v, std::shared_ptr<Link> l);
    */
    //std::shared_ptr<Link> link(std::string key, Port *target_port, std::string target_key);

    /*
    Port(std::string json_string);
    Port(bson_oid_t oid);
    Port(const char *uri, bson_oid_t oid);
     */
    //Port(std::shared_ptr<Node> node);
    //Port(std::string json_string, std::shared_ptr<Node> node);

};


#endif //chinet_PORT_H
