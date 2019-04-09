#ifndef FLUOMODLIB_PORT_H
#define FLUOMODLIB_PORT_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <cmath>

/*
#include <cstdlib>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/document/view_or_value.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
*/

#include "bson/bson.h"
#include "bson/bson-error.h"


#include <Node.h>


class Node;


class Port
        : public std::enable_shared_from_this<Port>{

    friend Node;

    static int sNextId;

private:

    size_t id;
    std::string name;

    std::shared_ptr<Node> node;
    std::shared_ptr<Port> input;
    std::vector<std::shared_ptr<Port>> targets;


public:

    /// stores the data of the Port in a BSON document
    //std::vector<uint8_t> data;
    bson_t *b;

    // Constructor
    //--------------------------------------------------------------------
    Port();
    Port(std::string name);
    Port(std::shared_ptr<Node> &node);
    //Port(std::string name, std::string filename);
    //Port(std::string name, std::string filename, std::shared_ptr<Node> &node);
    Port(std::string name, std::shared_ptr<Node> &node);

    // Destructor
    //--------------------------------------------------------------------
    //~Port();


    // Getter
    //--------------------------------------------------------------------
    std::string to_json();
    std::string get_name();
    int get_id();
    double get_value(const std::string &slot);
    std::shared_ptr<Port> shared_ptr();
    std::vector<std::string> get_slot_names();

    // Setter
    //--------------------------------------------------------------------
    void set_name(std::string &v);
    void set_slot_value(std::string slot_key, double v);

    // Operator
    //--------------------------------------------------------------------

    // Methods
    //--------------------------------------------------------------------
    void from_json(const std::string &json_string);

};


#endif //FLUOMODLIB_PORT_H
