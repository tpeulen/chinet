#include "Port.h"


// Methods
// --------------------------------------------------------------------


bool ValuePort::read_from_db(const std::string &oid_string)
{
    bool re = MongoObject::read_from_db(oid_string);
    _buff_double_vector = MongoObject::get_array<double>("value");
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
    bson_t dst = MongoObject::get_bson_excluding("value", NULL);
    MongoObject::append_number_array(&dst, "value", _buff_double_vector);
    return dst;
}

void ValuePort::set_value(double *in, int nbr_in)
{
#if CHINET_DEBUG
    std::clog << "Node:" << get_name() << ".set_value" << std::endl;
#endif
    _buff_double_vector.assign(in, in + nbr_in);
    if(node_ != nullptr){
        node_->set_node_to_invalid();
        if(is_reactive() && !is_output()){
            node_->evaluate();
        }
    }
}

void ValuePort::set_value(double value)
{
    auto v = std::vector<double>{value};
    set_value(v.data(), v.size());
}

void Port::set_value(double *in, int nbr_in)
{
#if CHINET_DEBUG
    std::clog << "Node:" << get_name() << ".set_value" << std::endl;
#endif
    ValuePort::set_value(in, nbr_in);
    port_links.set_value_of_dependents(in, nbr_in);
}

void Port::set_value(double value)
{
    auto v = std::vector<double>{value};
    set_value(v.data(), v.size());
}


// Getter
//--------------------------------------------------------------------

void ValuePort::get_value(double **out, int *nbr_out)
{
    if (_buff_double_vector.empty()) {
        _buff_double_vector = MongoObject::get_array<double>("value");
    }
    *nbr_out = _buff_double_vector.size();
    *out = _buff_double_vector.data();
}

void Port::get_value(double **out, int *nbr_out)
{
    if (!port_links.is_linked()) {
        ValuePort::get_value(out, nbr_out);
    } else {
        port_links.get_link()->get_value(out, nbr_out);
    }
}

std::vector<double> ValuePort::get_value()
{
    double *out; int n_out;
    get_value(&out, &n_out);

    std::vector<double> v{};
    v.assign(out, out+n_out);
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

bool BasePort::is_output()
{
    return get_singleton<bool>("is_output");
}

bool BasePort::is_reactive()
{
    return get_singleton<bool>("is_reactive");
}

void BasePort::set_fixed(bool fixed)
{
    set_singleton("is_fixed", fixed);
}

void BasePort::set_port_type(bool is_output)
{
    set_singleton("is_output", is_output);
}

void BasePort::set_reactive(bool reactive)
{
    set_singleton("is_reactive", reactive);
}

Node* BasePort::get_node()
{
    return node_;
}

void BasePort::set_node(Node *node_ptr)
{
    node_ = node_ptr;
}