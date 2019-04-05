#include "Port.h"

int Port::sNextId = 0;


// Constructor
Port::Port(
        std::shared_ptr<Node> &node,
        std::vector<std::uint8_t> &v_bson_value
        ) :
        Port("", node, v_bson_value)
{}


Port::Port(
        std::string name,
        std::shared_ptr<Node> &node,
        std::vector<std::uint8_t> &v_bson_value
        ) :
        Port(name)
{
    this->node = node;
    Port::v_bson_value = v_bson_value;
}


Port::Port(){
    id = sNextId++;
}


Port::Port(std::string name) : Port(){
    this->name = name;
}


// Getter
int Port::get_id() {
    return id;
}

std::string Port::get_name(){
    return name;
}

std::string Port::get_json(){
    if(!v_bson_value.empty()){
        size_t size;
        char* data = bson_as_json(b, &size);
        std::string json_string(data, size);
        return json_string;
        //return json::from_bson(v_bson_value).dump();
    } else return "";
}

std::shared_ptr<Port> Port::shared_ptr() {
    return std::shared_ptr<Port>(this);
}

// Setter
void Port::set_name(std::string &v){
    name = v;
}

void Port::set_value(std::vector <std::uint8_t> &v){
    v_bson_value = v;
}


// Methods
std::vector<std::uint8_t> Port::get_value(){
    if(input == nullptr){
        return v_bson_value;
    } else{
        return input->v_bson_value;
    }
}

void Port::read_json(std::string json_string){
    bson_json_reader;
    json j = json::parse(json_string);
    v_bson_value = json::to_bson(j);
}

