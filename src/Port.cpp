#include "Port.h"

// Operator
// --------------------------------------------------------------------
ValuePort ValuePort::operator+(ValuePort &v)
{
    std::vector<double> a = this->get_value();
    std::vector<double> b = v.get_value();
    auto result = Functions::get_vector_of_min_size(a, b);

    for(int i = 0; i < result.size(); ++i){
        result[i] = a[i] + b[i];
    }

    ValuePort re;
    re.set_name(this->get_name()  + "+" + v.get_name());
    re.set_value(result.data(), result.size());

    return re;
}

ValuePort ValuePort::operator-(ValuePort &v)
{
    std::vector<double> a = this->get_value();
    std::vector<double> b = v.get_value();
    auto result = Functions::get_vector_of_min_size(a, b);

    for(int i = 0; i < result.size(); ++i){
        result[i] = a[i] - b[i];
    }

    ValuePort re;
    re.set_name(this->get_name()  + "-" + v.get_name());
    re.set_value(result.data(), result.size());

    return re;
}

ValuePort ValuePort::operator*(ValuePort &v)
{
    std::vector<double> a = this->get_value();
    std::vector<double> b = v.get_value();
    auto result = Functions::get_vector_of_min_size(a, b);

    for(int i = 0; i < result.size(); ++i){
        result[i] = a[i] * b[i];
    }

    ValuePort re;
    re.set_name(this->get_name()  + "*" + v.get_name());
    re.set_value(result.data(), result.size());

    return re;
}


ValuePort ValuePort::operator/(ValuePort &v)
{
    std::vector<double> a = this->get_value();
    std::vector<double> b = v.get_value();
    auto result = Functions::get_vector_of_min_size(a, b);

    for(int i = 0; i < result.size(); ++i){
        result[i] = a[i] / b[i];
    }

    ValuePort re;
    re.set_name(this->get_name()  + "/" + v.get_name());
    re.set_value(result.data(), result.size());

    return re;
}


// Methods
// --------------------------------------------------------------------
bool ValuePort::read_from_db(const std::string &oid_string)
{
    bool re = MongoObject::read_from_db(oid_string);
    value_buffer_ = MongoObject::get_array<double>("value");
    return re;
}

bool ValuePort::write_to_db()
{
    bson_t doc = get_bson();
    return MongoObject::write_to_db(doc, 0);
}

// Setter
//--------------------------------------------------------------------
bson_t ValuePort::get_bson()
{
    bson_t dst = get_bson_excluding("value", "bounds", NULL);
    append_number_array(&dst, "value", value_buffer_);
    append_number_array(&dst, "bounds", bounds_);
    return dst;
}

bool ValuePort::bound_is_valid()
{
    if (bounds_.size() == 2) {
        if (bounds_[0] != bounds_[1]) {
            return true;
        }
    }
    return false;
}

void ValuePort::set_bounds(double lower, double upper)
{
    bounds_.resize(2);
    bounds_[0] = MIN(lower, upper);
    bounds_[1] = MAX(lower, upper);

    // adjust values to be within bounds
    /*
    Functions::map_to_bounds<double>(
            value_buffer_.data(), value_buffer_.size(),
            bounds_[0], bounds_[1]
    );
     */

    for (auto &v : value_buffer_) {
        v = MAX(v, bounds_[0]);
        v = MIN(v, bounds_[1]);
    }
}

void ValuePort::set_bounds(std::vector<double> bounds)
{
    if (bounds.size() >= 2) {
        set_bounds(
                MIN(bounds[0], bounds[1]),
                MAX(bounds[0], bounds[1])
        );
    }
}

void ValuePort::get_bounds(double **out, int *n_out)
{
    *out = bounds_.data();
    *n_out = bounds_.size();
}

std::vector<double> ValuePort::get_bounds()
{
    return bounds_;
}

void ValuePort::set_value(double *in, int n_int)
{
#if CHINET_DEBUG
    std::clog << "ValuePort:" << get_name() << ".set_value" << std::endl;
#endif

    if (is_bounded() && bound_is_valid()) {
#if CHINET_DEBUG
        std::clog << "bound values to: (" << bounds_[0] << ", " << bounds_[1] << ")" << std::endl;
#endif
        Functions::map_to_bounds<double>(
                in, n_int,
                bounds_[0], bounds_[1]
        );
    }
    value_buffer_.assign(in, in + n_int);
    if (node_ != nullptr) {
#if CHINET_DEBUG
        std::clog << "Port is attached to node:" << node_->get_name() << std::endl;
#endif
        node_->set_node_to_invalid();
        if (is_reactive() && !is_output()) {
            node_->evaluate();
        }
    }
}

void ValuePort::set_value(double value)
{
    auto v = std::vector<double>{value};
    set_value(v.data(), v.size());
}

void Port::set_value(double *in, int n_int)
{
#if CHINET_DEBUG
    std::clog << "Port:" << get_name() << ".set_value" << std::endl;
#endif
    ValuePort::set_value(in, n_int);
    port_links.set_value_of_dependents(in, n_int);
}

void Port::set_value(double value)
{
    auto v = std::vector<double>{value};
    set_value(v.data(), v.size());
}


// Getter
//--------------------------------------------------------------------
void ValuePort::get_value(double **out, int *n_out)
{
    if (value_buffer_.empty()) {
        value_buffer_ = MongoObject::get_array<double>("value");
    }
    *n_out = value_buffer_.size();
    *out = value_buffer_.data();
}

void Port::get_value(double **out, int *n_out)
{
    if (!port_links.is_linked()) {
        ValuePort::get_value(out, n_out);
    } else {
        port_links.get_link()->get_value(out, n_out);
    }
}

std::vector<double> ValuePort::get_value()
{
    double *out;
    int n_out;
    get_value(&out, &n_out);

    std::vector<double> v{};
    v.assign(out, out + n_out);
    return v;
}

std::vector<double> Port::get_value()
{
    return ValuePort::get_value();
}

bool BasePort::is_fixed()
{
    return get_singleton<bool>("is_fixed");
}

void BasePort::set_fixed(bool fixed)
{
    set_singleton("is_fixed", fixed);
}

bool BasePort::is_output()
{
    return get_singleton<bool>("is_output");
}

void BasePort::set_port_type(bool is_output)
{
    set_singleton("is_output", is_output);
}

bool BasePort::is_reactive()
{
    return get_singleton<bool>("is_reactive");
}

void BasePort::set_reactive(bool reactive)
{
    set_singleton("is_reactive", reactive);
}

bool ValuePort::is_bounded()
{
    return get_singleton<bool>("is_bounded");
}

void ValuePort::set_bounded(bool bounded)
{
    set_singleton("is_bounded", bounded);
}

Node *BasePort::get_node()
{
    return node_;
}

void BasePort::set_node(Node *node_ptr)
{
    node_ = node_ptr;
}
