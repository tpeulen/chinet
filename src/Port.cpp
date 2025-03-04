#include "Port.h"

std::shared_ptr<Port> Port::get_ptr() {
    return std::dynamic_pointer_cast<Port>(shared_from_this());
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
    bson_t doc = get_bson();
    return MongoObject::write_to_db(doc, 0);
}

bool Port::read_from_db(const std::string &oid_string) {
    bool re = MongoObject::read_from_db(oid_string);
    auto v = MongoObject::get_array<uint8_t>("value");
    buffer_ = v;
    return re;
}

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


template<typename T>
void Port::set_value(T *input, int n_input, bool copy_values) {
    if (is_fixed()) {
        return;
    }
    if (copy_values) {
        buffer_.resize(n_input * sizeof(T));
        std::memcpy(buffer_.data(), input, n_input * sizeof(T));
    } else {
        buffer_ = std::vector<uint8_t>(reinterpret_cast<uint8_t*>(input),
                                       reinterpret_cast<uint8_t*>(input) + n_input * sizeof(T));
    }
    buffer_element_size_ = sizeof(T);
    if (node_ != nullptr) {
        update_attached_node();
        set_value_of_dependents(input, n_input);
    }
}

template<typename T>
void Port::get_value(T **output, int *n_output) {
    *n_output = buffer_.size() / sizeof(T);
    *output = reinterpret_cast<T*>(buffer_.data());
}

template<typename T>
void Port::get_own_value(T **output, int *n_output) {
    if (buffer_.empty()) {
        update_buffer<T>();
    }
    *n_output = buffer_.size() / sizeof(T);
    *output = reinterpret_cast<T*>(buffer_.data());
}

template<typename T>
void Port::update_buffer() {
    auto v = get_array<T>("value");
    buffer_.resize(v.size() * sizeof(T));
    std::memcpy(buffer_.data(), v.data(), v.size() * sizeof(T));
}

void Port::update_attached_node() {
    node_->set_valid(false);
    if (is_reactive() && !is_output()) {
        node_->evaluate();
    }
}
