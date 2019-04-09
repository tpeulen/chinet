#include "Port.h"

int Port::sNextId = 0;


// Constructor
//--------------------------------------------------------------------
Port::Port(
        std::shared_ptr<Node> &node
        ) :
        Port("", node)
{
    bson_init(b);
}


Port::Port(
        std::string name,
        std::shared_ptr<Node> &node
        ) :
        Port(name)
{
    this->node = node;
}


Port::Port()
{
    id = sNextId++;
}


Port::Port(std::string name) : Port(){
    this->name = name;
}

// Destructor
//--------------------------------------------------------------------


// Operator
//--------------------------------------------------------------------


// Getter
//--------------------------------------------------------------------
int Port::get_id() {
    return id;
}

std::string Port::get_name(){
    return name;
}

std::string Port::to_json(){
    if(b == nullptr){
        return "";
    } else{
        size_t len;
        char* str = bson_as_json (b, &len);
        return std::string(str, len);
    }
}

std::shared_ptr<Port> Port::shared_ptr() {
    return std::shared_ptr<Port>(this);
}

// Setter
//--------------------------------------------------------------------
void Port::set_name(std::string &v){
    name = v;
}

void Port::set_slot_value(std::string slot_key, double value){
    /*
    // Todo: this is very inefficient
    auto doc = bsoncxx::builder::stream::document{};
    auto v = bsoncxx::document::view(data.data(), data.size());
    for(auto i : v){
        if(i.key() != slot_key){
            doc << i.key() << i.get_value();
        } else{
            for(){

            }
            doc << i.key() << value;
        }
    }

    size_t n = doc.view().length();
    data.clear(); data.resize(n);
    for(int i=0; i < n; i++){
        data[i] = doc.view().data()[i];
    }
     */

}


// Methods
// --------------------------------------------------------------------
double Port::get_value(const std::string &slot_key){
    bson_iter_t iter;
    bson_iter_t baz;

    if (bson_iter_init (&iter, b) &&
        bson_iter_find_descendant (&iter, "foo.bar.0.baz", &baz) &&
            BSON_ITER_HOLDS_DOUBLE (&baz)) {
        printf ("baz = %d\n", bson_iter_int32 (&baz));
    }
    return 0;

    /*
    auto view = bsoncxx::document::view(data.data(), data.size());
    auto sv = bsoncxx::stdx::string_view(slot_key.c_str());
    //auto sv = std::string_view(slot_key);
    try{
        auto element = view[sv]["value"];
        if(element.type() != bsoncxx::type::k_double) {
            throw bsoncxx::exception();
        }
        return element.get_double();
    }
    catch(bsoncxx::exception &e){
        std::cerr <<
        "An exception occurred: They value of Port " <<
        get_name() << "." << slot_key <<
        " is not valid.\n";
        return NAN;
    }
     */
}

std::vector<std::string> Port::get_slot_names(){
    /*
    std::vector<std::string> names;
    auto view = bsoncxx::document::view(data.data(), data.size());
    for(bsoncxx::document::element ele : view){
        names.push_back(ele.key().to_string());
    }
    return names;
     */
}

void Port::from_json(const std::string &json_string){
    bson_error_t error;
    b = bson_new_from_json(
            (uint8_t*)json_string.c_str(),
            json_string.size(),
            &error
            );
    if (!b) {
        printf ("Error: %s\n", error.message);
    }
}

