#ifndef FLUOMODLIB_PORT_H
#define FLUOMODLIB_PORT_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <cmath>
#include <chrono>
#include "bson.h"

#include <Functions.h>
#include <Node.h>


class Node;


class Port : public std::enable_shared_from_this<Port>{

    friend Node;

private:

    //std::shared_ptr<Node> node;
    std::shared_ptr<Port> input;
    std::vector<std::shared_ptr<Port>> targets;

    /// stores the data of the Port in a BSON document
    bson_t *b;

    /// the object identifier (unique number)
    bson_oid_t oid;

public:

    // Constructor
    //--------------------------------------------------------------------
    Port();
    Port(std::string json_string);
    Port(bson_oid_t oid);
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
    double get_slot_value(const std::string &slot_key);
    /// returns the values of all the slots as an vector
    std::vector<double> get_slot_values();
    /// returns the keys of all the slots as an vector
    std::vector<std::string> get_slot_keys();
    std::shared_ptr<Port> shared_ptr();
    std::string get_oid();

    // Setter
    //--------------------------------------------------------------------
    void set_input(std::shared_ptr<Port> v);
    bool set_slot_value(std::string slot_key, double value);
    void set_oid(std::string v);
    //void set_node(std::shared_ptr<Node> node);
    void set_predecessor(bson_oid_t v);

    // Operator
    //--------------------------------------------------------------------

    // Methods
    //--------------------------------------------------------------------
    void from_json(const std::string &json_string);
    bool add_port_to_collection(mongoc_collection_t * port_collection);
    void new_oid();

};


#endif //FLUOMODLIB_PORT_H
