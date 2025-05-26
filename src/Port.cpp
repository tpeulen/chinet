#include "Port.h"

std::shared_ptr<Port> Port::get_ptr() {
    return std::dynamic_pointer_cast<Port>(shared_from_this());
}

std::shared_ptr<Port> Port::operator+(std::shared_ptr<Port> v)
{
#if CHINET_VERBOSE
    std::clog << "ADDING PORTS" << std::endl;
#endif
    int new_value_type = std::max(value_type, v->value_type);
#if CHINET_VERBOSE
    std::clog << "-- Value type of resulting port: " << new_value_type << std::endl;
#endif
    std::string name = get_name()  + " + " + v->get_name();
#if CHINET_VERBOSE
    std::clog << "-- Name of resulting port: " << name << std::endl;
#endif
    auto re = std::make_shared<Port>(
            false, true, true, false, 0, 0, new_value_type, name
    );
#if CHINET_VERBOSE
    std::clog << "-- Creating a Node associated to the resulting port." << std::endl;
#endif
    auto node = new Node();
    node->set_name(name);
    node->add_input_port(this->get_name(), get_ptr());
    node->add_input_port(v->get_name(), v);
    node->add_output_port(name, re);
    if(new_value_type == 0){
        node->set_callback("addition_int", "C");
    } else if(new_value_type == 1){
        node->set_callback("addition_double", "C");
    }
    re->set_node(node);
    node->evaluate();
    return re;
}


std::shared_ptr<Port> Port::operator*(std::shared_ptr<Port> v)
{
    auto re = std::make_shared<Port>();
    int new_value_type = std::max(this->get_value_type(), v->get_value_type());
    re->set_value_type(new_value_type);
    auto node = new Node();
    std::string name = this->get_name()  + "*" + v->get_name();
    node->set_name(name);
    node->add_input_port(this->get_name(), get_ptr());
    node->add_input_port(v->get_name(), v);
    node->add_output_port(name, re);
    if(new_value_type == 0){
        node->set_callback("multiply_int", "C");
    } else if(new_value_type == 1){
        node->set_callback("multiply_double", "C");
    }
    re->set_node(node);
    re->set_name(name);
    re->set_port_type(true);
    node->evaluate();
    return re;
}


void Port::set_link(std::shared_ptr<Port> v) {
    if (v == nullptr) {
        unlink();
        return;
    }
    unlink();
    link_ = v;
    v->linked_to_.push_back(this);
    if (node_ != nullptr) update_attached_node();
}

bool Port::write_to_db() {
#ifdef WITH_MONGODB
    bson_t doc = get_bson();
    return MongoObject::write_to_db(doc, 0);
#else
    return DatabaseObject::write_to_db();
#endif
}

bool Port::read_from_db(const std::string &oid_string) {
    bool re = DatabaseObject::read_from_db(oid_string);
#ifdef WITH_MONGODB
    auto v = MongoObject::get_array<uint8_t>("value");
#else
    auto v = get_array<uint8_t>("value");
#endif
    buffer_ = v;
    return re;
}

#ifdef WITH_MONGODB
bson_t Port::get_bson()
{
    bson_t dst = get_bson_excluding("value", "bounds", NULL);
    if(value_type == 0){
        long* va; int nv;
        get_own_value(&va, &nv);
        auto v = std::vector<long>();
        v.assign(va, va + nv);
        append_number_array(&dst, "value", v);
    } else{
        double* va; int nv;
        get_own_value(&va, &nv);
        auto v = std::vector<double>();
        v.assign(va, va + nv);
        append_number_array(&dst, "value", v);
    }
    append_number_array(&dst, "bounds", bounds_);
    return dst;
}
#endif


bool Port::bound_is_valid()
{
    if (bounds_.size() == 2) {
        if (bounds_[0] != bounds_[1]) {
            return true;
        }
    }
    return false;
}


void Port::set_bounds(std::vector<double> v)
{
    if (v.size() >= 2) {
        bounds_.clear();
        double lower = std::min(v[0], v[1]);
        double upper = std::max(v[0], v[1]);
        bounds_.push_back(lower);
        bounds_.push_back(upper);
    }
}


std::vector<double> Port::get_bounds()
{
    return bounds_;
}


void Port::update_attached_node() {
    node_->set_valid(false);
    if (is_reactive() && !is_output()) {
        node_->evaluate();
    }
}

void Port::get_bytes(unsigned char **output, int *n_output, bool copy) {
    *n_output = buffer_.size();
    if (copy) {
        auto buffer_size = *n_output;
        *output = static_cast<unsigned char*>(std::malloc(buffer_size)); // Use malloc for improved performance
        if (*output) {
            std::memcpy(*output, buffer_.data(), buffer_size);
        } else {
            throw std::runtime_error("Memory allocation failed in get_bytes.");
        }
    } else {
        *output = buffer_.data();
    }
}

void Port::set_bytes(unsigned char *input, int n_input) {
    buffer_.resize(n_input);
    std::memcpy(buffer_.data(), input, n_input);
    buffer_element_size_ = 1; // When setting raw bytes, each element is 1 byte
}

void Port::set_buffer_ptr(size_t ptr, int n_elements, int element_size) {
    buffer_.assign(reinterpret_cast<uint8_t*>(ptr),
                   reinterpret_cast<uint8_t*>(ptr) + (n_elements * element_size));
    buffer_element_size_ = element_size;
}

size_t Port::get_buffer_ptr() {
    return reinterpret_cast<size_t>(buffer_.data());
}

std::vector<Port *> Port::get_linked_ports() {
    return linked_to_;
}
