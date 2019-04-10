#ifndef FLUOMODLIB_PORT_H
#define FLUOMODLIB_PORT_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <cmath>

#include "bson.h"
#include "bson/bson-error.h"

#include <Node.h>


class Node;


class Port
        : public std::enable_shared_from_this<Port>{

    friend Node;

private:

    std::string name;

    std::shared_ptr<Node> node;
    std::shared_ptr<Port> input;
    std::vector<std::shared_ptr<Port>> targets;

    /// stores the data of the Port in a BSON document
    bson_t *b;

    /// the object identifier (unique number)
    bson_oid_t oid;

public:

    // Constructor
    //--------------------------------------------------------------------
    Port() = default;
    Port(std::string name);
    Port(std::shared_ptr<Node> &node);
    //Port(std::string name, std::string filename);
    //Port(std::string name, std::string filename, std::shared_ptr<Node> &node);
    Port(std::string name, std::shared_ptr<Node> &node);

    // Destructor
    //--------------------------------------------------------------------
    ~Port();


    // Getter
    //--------------------------------------------------------------------
    std::string to_json();
    std::string get_name();
    double get_slot_value(const std::string &slot_key);
    std::shared_ptr<Port> shared_ptr();
    std::vector<std::string> get_slot_names();
    std::string get_oid();

    // Setter
    //--------------------------------------------------------------------
    void set_name(std::string &v);
    void set_slot_value(std::string slot_key, double value);
    void set_oid(std::string v);

    // Operator
    //--------------------------------------------------------------------

    // Methods
    //--------------------------------------------------------------------
    void from_json(const std::string &json_string);

};


#endif //FLUOMODLIB_PORT_H
