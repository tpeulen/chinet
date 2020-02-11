#include "Port.h"

// Operator
// --------------------------------------------------------------------
Port* Port::operator+(Port* v)
{
    auto re = new Port();
    re->set_value_type(this->get_value_type());
    auto node = new Node();
    std::string name = this->get_name()  + "+" + v->get_name();
    node->set_name(name);
    node->add_input_port(this->get_name(), this);
    node->add_input_port(v->get_name(), v);
    node->add_output_port(name, re);
    if(this->get_value_type() == 0){
        node->set_callback("addition_double", "C");
    } else if(this->get_value_type() == 1){
        node->set_callback("addition_int", "C");
    }
    re->set_node(node);
    re->set_name(name);
    re->set_port_type(true);
    node->evaluate();
    return re;
}

Port* Port::operator*(Port* v)
{
    auto re = new Port();
    re->set_value_type(this->get_value_type());
    auto node = new Node();
    std::string name = this->get_name()  + "+" + v->get_name();
    node->set_name(name);
    node->add_input_port(this->get_name(), this);
    node->add_input_port(v->get_name(), v);
    node->add_output_port(name, re);
    if(this->get_value_type() == 0){
        node->set_callback("multiply_double", "C");
    } else if(this->get_value_type() == 1){
        node->set_callback("multiply_int", "C");
    }
    re->set_node(node);
    re->set_name(name);
    re->set_port_type(true);
    node->evaluate();
    return re;
}

//
//Port Port::operator-(Port &v)
//{
//    auto a = get_value_vector<double>();
//    auto b = v.get_value_vector<double>();
//    auto result = Functions::get_vector_of_min_size(a, b);
//
//    for(int i = 0; i < result.size(); ++i){
//        result[i] = a[i] - b[i];
//    }
//
//    Port re(result);
//    re.set_name(this->get_name()  + "-" + v.get_name());
//
//    return re;
//}
//
//Port Port::operator*(Port &v)
//{
//    auto a = get_value_vector<double>();
//    auto b = v.get_value_vector<double>();
//    auto result = Functions::get_vector_of_min_size(a, b);
//
//    for(int i = 0; i < result.size(); ++i){
//        result[i] = a[i] * b[i];
//    }
//
//    Port re(result);
//    re.set_name(this->get_name()  + "*" + v.get_name());
//
//    return re;
//}
//
//
//Port Port::operator/(Port &v)
//{
//    auto a = get_value_vector<double>();
//    auto b = v.get_value_vector<double>();
//    auto result = Functions::get_vector_of_min_size(a, b);
//
//    for(int i = 0; i < result.size(); ++i){
//        result[i] = a[i] / b[i];
//    }
//
//    Port re(result);
//    re.set_name(this->get_name()  + "/" + v.get_name());
//
//    return re;
//}


// Methods
// --------------------------------------------------------------------
bool Port::read_from_db(const std::string &oid_string)
{
    bool re = MongoObject::read_from_db(oid_string);
    auto v = MongoObject::get_array<double>("value");
    n_buffer_ = v.size();
    buffer_ = std::realloc(buffer_, sizeof(double) * n_buffer_);
    if(buffer_ != nullptr)
    {
        n_buffer_ = v.size();
        if(value_type == 0){
            auto t = reinterpret_cast<double*>(buffer_);
            for(int i=0; i < n_buffer_; i++){
                t[i] = v[i];
            }
        } else{
            auto t = reinterpret_cast<int*>(buffer_);
            for(int i=0; i < n_buffer_; i++){
                t[i] = (int) v[i];
            }
        }
    }
    return re;
}

bool Port::write_to_db()
{
    bson_t doc = get_bson();
    return MongoObject::write_to_db(doc, 0);
}

// Setter
//--------------------------------------------------------------------

bson_t Port::get_bson()
{
    bson_t dst = get_bson_excluding("value", "bounds", NULL);
    if(value_type == 0){
        double* va; int nv;
        get_own_value(&va, &nv);
        auto v = std::vector<double>();
        v.assign(va, va + nv);
        append_number_array(&dst, "value", v);
    } else{
        long* va; int nv;
        get_own_value(&va, &nv);
        auto v = std::vector<long>();
        v.assign(va, va + nv);
        append_number_array(&dst, "value", v);
    }
    append_number_array(&dst, "bounds", bounds_);
    return dst;
}

bool Port::bound_is_valid()
{
    if (bounds_.size() == 2) {
        if (bounds_[0] != bounds_[1]) {
            return true;
        }
    }
    return false;
}

void Port::set_bounds(double lower, double upper)
{
    bounds_.resize(2);
    bounds_[0] = MIN(lower, upper);
    bounds_[1] = MAX(lower, upper);
}

void Port::set_bounds(std::vector<double> bounds)
{
    if (bounds.size() >= 2) {
        set_bounds(
                MIN(bounds[0], bounds[1]),
                MAX(bounds[0], bounds[1])
        );
    }
}

std::vector<double> Port::get_bounds()
{
    return bounds_;
}




// Getter
//--------------------------------------------------------------------


bool Port::is_fixed()
{
    return get_singleton<bool>("is_fixed");
}

void Port::set_fixed(bool fixed)
{
    set_singleton("is_fixed", fixed);
}

bool Port::is_output()
{
    return get_singleton<bool>("is_output");
}

void Port::set_port_type(bool is_output)
{
    set_singleton("is_output", is_output);
}

bool Port::is_reactive()
{
    return get_singleton<bool>("is_reactive");
}

void Port::set_reactive(bool reactive)
{
    set_singleton("is_reactive", reactive);
}

bool Port::is_bounded()
{
    return get_singleton<bool>("is_bounded");
}

void Port::set_bounded(bool bounded)
{
    set_singleton("is_bounded", bounded);
}

Node* Port::get_node()
{
    return node_;
}

void Port::set_node(Node* node_ptr)
{
    node_ = node_ptr;
}

void Port::update_attached_node() {
#if DEBUG
    std::clog << "Port caused update of node with name: " << node_->get_name() << std::endl;
#endif
    node_->set_node_to_invalid();
    if (is_reactive() && !is_output()) {
        node_->evaluate();
    }
}