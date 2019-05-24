#include "Port.h"


// Methods
// --------------------------------------------------------------------


bool Port::is_fixed(){
    return MongoObject::get_singleton<bool>("is_fixed");
}

bool Port::is_linked(){
    return (link_ != nullptr);
}

bool Port::is_output()
{
    return MongoObject::get_singleton<bool>("is_output");
}

bool Port::is_reactive(){
    return _is_reactive;
}

void Port::link(std::shared_ptr<Port> &v){
    if(v != nullptr){
        MongoObject::set_oid("link", v->get_bson_oid());
        link_ = v;
        v->linked_to_.push_back(this);
    }
}

bool Port::unlink(){
    bool re = true;
    MongoObject::set_oid("link", get_bson_oid());
    re &= remove_pointer_to_this_in_link_port();
    // set the link to a nullptr
    link_ = nullptr;
    return re;
}


bool Port::remove_pointer_to_this_in_link_port(){
    if(link_ != nullptr){
        // remove pointer to this port in the port to which this is linked
        auto it = std::find(link_->linked_to_.begin(),
                            link_->linked_to_.end(), this);

        if(it != link_->linked_to_.end()){
            link_->linked_to_.erase(it);
            return true;
        }
    }
    return false;
}

bool Port::read_from_db(const std::string &oid_string)
{
    bool re = MongoObject::read_from_db(oid_string);
    _buff_double_vector = MongoObject::get_array<double>("value");
    return re;
}

bool Port::write_to_db()
{
    bson_t doc = get_bson();
    return MongoObject::write_to_db(doc, 0);
}


// Setter
//--------------------------------------------------------------------

bson_t Port::get_bson() {
    bson_t dst = MongoObject::get_bson_excluding("value", NULL);
    MongoObject::append_number_array(&dst, "value", _buff_double_vector);
    return dst;
}

void Port::set_fixed(bool fixed){
    MongoObject::set_singleton("is_fixed", fixed);
}

void Port::set_port_type(bool is_output)
{
    MongoObject::set_singleton("is_output", is_output);
}

void Port::set_reactive(bool reactive){
    MongoObject::set_singleton("is_reactive", reactive);
    _is_reactive = reactive;
}

void Port::set_value(double *in, int nbr_in)
{
#if CHINET_DEBUG
    std::clog << "Node:" << get_name() << ".set_value" << std::endl;
#endif
    _buff_double_vector.assign(in, in + nbr_in);
    for(auto &v : linked_to_){
        v->set_value(in, nbr_in);
    }
    if(node_ != nullptr){
        node_->set_node_to_invalid();
        if(_is_reactive && !is_output()){
            node_->evaluate();
        }
    }
}

void Port::set_value(std::vector<double> &values)
{
    set_value(values.data(), values.size());
}

void Port::set_value(double value)
{
    auto v = std::vector<double>{value};
    set_value(v.data(), v.size());
}


void Port::set_node(Node *node_ptr){
    node_ = node_ptr;
}


// Getter
//--------------------------------------------------------------------

void Port::get_value(double **out, int *nbr_out)
{
    if (link_ == nullptr) {
        if (_buff_double_vector.empty()) {
            _buff_double_vector = MongoObject::get_array<double>("value");
        }
        *nbr_out = _buff_double_vector.size();
        *out = _buff_double_vector.data();
    } else {
        link_->get_value(out, nbr_out);
    }
}

std::vector<double> Port::get_value()
{
    if (link_ == nullptr) {
        if (_buff_double_vector.empty()) {
            _buff_double_vector = MongoObject::get_array<double>("value");
        }
        return _buff_double_vector;
    } else {
        return link_->get_value();
    }
}

std::vector<Port*> Port::get_linked_ports(){
    return linked_to_;
}

std::shared_ptr<Port> Port::get_link(){
    return link_;
}

Node* Port::get_node(){
    return node_;
}


