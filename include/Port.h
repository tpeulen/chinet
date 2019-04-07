#ifndef FLUOMODLIB_PORT_H
#define FLUOMODLIB_PORT_H

#include <memory>
#include <vector>
#include <string>
#include <Node.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <mongoc/mongoc.h>
#include <bsoncxx/json.hpp>


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

    /// stores the data of the Port in a BSON document
    bson_t* bson_doc;
    bson_iter_t bson_iter;

public:

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
    std::string get_json();
    std::string get_name();
    int get_id();
    bson_t* get_value();
    std::shared_ptr<Port> shared_ptr();

    // Setter
    //--------------------------------------------------------------------
    void set_name(std::string &v);
    void set_value(bson_t *v);

    // Operator
    //--------------------------------------------------------------------
    void operator[](std::string key);

    // Methods
    //--------------------------------------------------------------------
    void read_json(std::string json_string);

};


#endif //FLUOMODLIB_PORT_H
