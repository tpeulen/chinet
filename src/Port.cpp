#include "Port.h"

int Port::sNextId = 0;


// Constructor
Port::Port(
        std::shared_ptr<Node> &node
        ) :
        Port("", node)
{}


Port::Port(
        std::string name,
        std::shared_ptr<Node> &node
        ) :
        Port(name)
{
    this->node = node;
}


Port::Port(){
    id = sNextId++;
    bson_init(&b);
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
    char* data = bson_as_json(&b, NULL);
    std::string json_string(data);
    return json_string;
}

std::shared_ptr<Port> Port::shared_ptr() {
    return std::shared_ptr<Port>(this);
}

// Setter
void Port::set_name(std::string &v){
    name = v;
}

void Port::set_value(bson_t v){
    bson_destroy(&b);
    b = *bson_copy(&v);
}


// Methods
bson_t* Port::get_value(){
    if(input == nullptr){
        return bson_copy(&b);
    } else{
        return bson_copy(&(input->b));
    }
}

void Port::read_json(std::string json_string){
    bson_error_t error;
    bson_t *temp = bson_new_from_json((uint8_t*) json_string.c_str(), -1, &error);
    bson_destroy(&b);
    b = *bson_copy(temp);
}

