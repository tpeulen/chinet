#include "Port.h"

int Port::sNextId = 0;


// Constructor
//--------------------------------------------------------------------
Port::Port(
        std::shared_ptr<Node> &node
        ) :
        Port("", node)
{
    bson_iter_init(&bson_iter, bson_doc);
}


Port::Port(
        std::string name,
        //std::string filename,
        std::shared_ptr<Node> &node
        ) :
        Port(name)
{
    this->node = node;
}


Port::Port() {
    id = sNextId++;
    bson_init(bson_doc);
}


Port::Port(std::string name) : Port(){
    this->name = name;
}

// Destructor
//--------------------------------------------------------------------
/*
Port::~Port() :
std::enable_shared_from_this<Port>::~Port()
{
    bson_destroy(bson_doc);
}
 */


// Operator
//--------------------------------------------------------------------
void Port::operator[](std::string key){
    // find key in bson_doc
    bson_iter_find(&bson_iter, key.c_str());

    const uint8_t *data = NULL;
    uint32_t len = 0;
    bson_iter_array(&bson_iter, &len, &data);

    bson_t * fSubArray = bson_new_from_data(data, len);
    bson_iter_t fIter;
    bson_iter_init(&fIter, fSubArray);

}


// Getter
//--------------------------------------------------------------------
int Port::get_id() {
    return id;
}

std::string Port::get_name(){
    return name;
}

std::string Port::get_json(){
    char* data = bson_as_json(bson_doc, NULL);
    std::string json_string(data);
    return json_string;
}

std::shared_ptr<Port> Port::shared_ptr() {
    return std::shared_ptr<Port>(this);
}

// Setter
//--------------------------------------------------------------------
void Port::set_name(std::string &v){
    name = v;
}

void Port::set_value(bson_t *v){
    bson_destroy(bson_doc);
    bson_doc = bson_copy(v);
}


// Methods
// --------------------------------------------------------------------
bson_t* Port::get_value(){
    if(input == nullptr){
        return bson_copy(bson_doc);
    } else{
        return bson_copy((input->bson_doc));
    }
}

void Port::read_json(std::string json_string){
    bson_error_t error;
    bson_t *temp = bson_new_from_json((uint8_t*) json_string.c_str(), -1, &error);
    bson_destroy(bson_doc);
    bson_doc = bson_copy(temp);
}

