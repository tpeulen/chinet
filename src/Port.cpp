#include "Port.h"

// Operator
// --------------------------------------------------------------------
Port Port::operator+(Port &v)
{
    auto a = get_value_vector<double>();
    auto b = v.get_value_vector<double>();
    auto result = Functions::get_vector_of_min_size(a, b);

    for(int i = 0; i < result.size(); ++i){
        result[i] = a[i] + b[i];
    }

    Port re(result);
    re.set_name(this->get_name()  + "+" + v.get_name());

    return re;
}

Port Port::operator-(Port &v)
{
    auto a = get_value_vector<double>();
    auto b = v.get_value_vector<double>();
    auto result = Functions::get_vector_of_min_size(a, b);

    for(int i = 0; i < result.size(); ++i){
        result[i] = a[i] - b[i];
    }

    Port re(result);
    re.set_name(this->get_name()  + "-" + v.get_name());

    return re;
}

Port Port::operator*(Port &v)
{
    auto a = get_value_vector<double>();
    auto b = v.get_value_vector<double>();
    auto result = Functions::get_vector_of_min_size(a, b);

    for(int i = 0; i < result.size(); ++i){
        result[i] = a[i] * b[i];
    }

    Port re(result);
    re.set_name(this->get_name()  + "*" + v.get_name());

    return re;
}


Port Port::operator/(Port &v)
{
    auto a = get_value_vector<double>();
    auto b = v.get_value_vector<double>();
    auto result = Functions::get_vector_of_min_size(a, b);

    for(int i = 0; i < result.size(); ++i){
        result[i] = a[i] / b[i];
    }

    Port re(result);
    re.set_name(this->get_name()  + "/" + v.get_name());

    return re;
}


// Methods
// --------------------------------------------------------------------
bool Port::read_from_db(const std::string &oid_string)
{
    bool re = MongoObject::read_from_db(oid_string);
    auto v = MongoObject::get_array<double>("value");
    n_buffer = v.size();
    buffer_ = std::realloc(buffer_, sizeof(double) * n_buffer);
    if(buffer_ != nullptr)
    {
        n_buffer = v.size();
        auto t = reinterpret_cast<double*>(buffer_);
        for(int i=0; i<n_buffer; i++){
            t[i] = v[i];
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
    auto v = get_value_vector<double>();
    append_number_array(&dst, "value", v);
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

template <typename T>
void Port::set_value(T *in, int n_int)
{
#if DEBUG
    std::clog << "Number of elements: " << n_int << std::endl;
    std::clog << "Port:" << get_name() << ".set_value" << std::endl;
#endif
    buffer_ = std::realloc(buffer_, n_int * sizeof(T));
    if(buffer_ != nullptr)
    {
        auto t = reinterpret_cast<T*>(buffer_);
        for(int i=0; i<n_int; i++){
            t[i] = in[i];
        }
        n_buffer = n_int;

        if (is_bounded() && bound_is_valid()) {
#if DEBUG
        std::clog << "bound values to: (" << bounds_[0] << ", " << bounds_[1] << ")" << std::endl;
#endif
            Functions::map_to_bounds<T>(
                    t, n_buffer,
                    bounds_[0], bounds_[1]
            );
        }
        if (node_ != nullptr) {
#if DEBUG
        std::clog << "Port is attached to node:" << node_->get_name() << std::endl;
#endif
            node_->set_node_to_invalid();
            if (is_reactive() && !is_output()) {
                node_->evaluate();
            }
        }
        set_value_of_dependents(in, n_int);
    }
}


// Getter
//--------------------------------------------------------------------
template <typename T>
void Port::get_value(T **out, int *n_out)
{
    if (!is_linked()) {
        if (n_buffer == 0) {
            auto v = MongoObject::get_array<T>("value");
            n_buffer = v.size();
            auto t = reinterpret_cast<T*>(buffer_);
            if(malloc(sizeof(T) * n_buffer))
                for(int i=0; i<n_buffer; i++){
                    t[i] = v[i];
                }
        }
        *out = reinterpret_cast<T*>(buffer_);
        *n_out = n_buffer;
    } else {
        get_link()->get_value(out, n_out);
    }
}

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

Node *Port::get_node()
{
    return node_;
}

void Port::set_node(Node *node_ptr)
{
    node_ = node_ptr;
}
