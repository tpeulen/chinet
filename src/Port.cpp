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

void Port::update_attached_node() {
    node_->set_valid(false);
    if (is_reactive() && !is_output()) {
        node_->evaluate();
    }
}
