#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>
#include <algorithm>
#include <math.h>

#include <Node.h>
#include <PortLinks.h>

class Node;
template <class T> class PortLinks;

class BasePort : public MongoObject
{

protected:
    Node *node_;

public:

    bool is_fixed();
    void set_fixed(bool fixed);

    bool is_output();
    void set_port_type(bool is_output);

    bool is_reactive();
    void set_reactive(bool reactive);

    BasePort(
        bool fixed = false,
        bool is_output = false,
        bool is_reactive = false
    ) :
    node_(nullptr)
    {
        append_string(&document, "type", "port");
        bson_append_bool(&document, "is_output", 9, false);
        bson_append_bool(&document, "is_fixed", 8, false);
        bson_append_bool(&document, "is_reactive", 11, false);

        set_fixed(fixed);
        set_port_type(is_output);
        set_reactive(is_reactive);
    }

    void set_node(Node *node_ptr);
    Node* get_node();
};

class ValuePort : public BasePort
{

private:
    std::vector<double> value_buffer_{};
    std::vector<double> bounds_{};

public:
    // Constructor & Destructor
    //--------------------------------------------------------------------
    ValuePort() : BasePort()
    {
        bson_append_oid(&document, "link", 4, &oid_document);
        bson_append_bool(&document, "is_bounded", 10, false);
    }

    ValuePort(double value,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false,
         bool is_bounded = false,
         double lower_bound = 0,
         double upper_bound = 0
    ) : BasePort(fixed, is_output, is_reactive)
    {
        set_value(value);
        set_bounded(is_bounded);
        if(is_bounded){
            set_bounds(lower_bound, upper_bound);
        }
    };

    ValuePort(std::vector<double> array,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false,
         bool is_bounded = false,
         double lower_bound = 0,
         double upper_bound = 0
    ) : BasePort(fixed, is_output, is_reactive)
    {
        set_value(array.data(), array.size());
        set_bounded(is_bounded);
        if(is_bounded){
            set_bounds(lower_bound, upper_bound);
        }
    };

    ~ValuePort(){
    };

    // Getter & Setter
    //--------------------------------------------------------------------
    virtual void set_value(double *in, int n_in);
    virtual void set_value(double value);
    virtual void get_value(double **out, int *n_out);
    virtual std::vector<double> get_value();
    virtual bson_t get_bson() final;

    // Methods
    //--------------------------------------------------------------------
    void set_bounded(bool bounded);
    bool bound_is_valid();
    bool is_bounded();
    void set_bounds(double lower, double upper);
    void set_bounds(std::vector<double> bound);
    void get_bounds(double **out, int *n_out);
    std::vector<double> get_bounds();

    bool write_to_db();
    bool read_from_db(const std::string &oid_string);

    // Operators
    //--------------------------------------------------------------------
    ValuePort operator+(ValuePort &v);
    ValuePort operator-(ValuePort &v);
    ValuePort operator*(ValuePort &v);
    ValuePort operator/(ValuePort &v);
};


class Port : public ValuePort
{

public:

    PortLinks<double> port_links;

    // Constructor & Destructor
    //--------------------------------------------------------------------

    Port() : ValuePort(), port_links(this)
    {
        bson_append_oid(&document, "link", 4, &oid_document);
    }

    Port(double value,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false,
         bool is_bounded = false,
         double lower_bound = 0,
         double upper_bound = 0
    ) : ValuePort(
            value,
            fixed,
            is_output, is_reactive,
            is_bounded, lower_bound, upper_bound), port_links(this)
    {
    };

    Port(std::vector<double> array,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false,
         bool is_bounded = false,
         double lower_bound = 0,
         double upper_bound = 0
    ) : ValuePort(array,
            fixed,
            is_output,
            is_reactive,
            is_bounded, lower_bound, upper_bound), port_links(this)
    {
    };

    ~Port(){
    };

    // Methods
    //--------------------------------------------------------------------
    void set_value(double *in, int n_in);
    void set_value(double value);
    void get_value(double **out, int *n_out);
    std::vector<double> get_value();

    void link(std::shared_ptr<Port> &v)
    {
        set_oid("link", v->get_bson_oid());
        port_links.link(v);
    }

    bool unlink()
    {
        set_oid("link", get_bson_oid());
        return port_links.unlink();
    }

};

#endif //chinet_PORT_H
