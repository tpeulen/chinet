#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>
#include <algorithm>

#include <Node.h>
#include <PortLinks.h>

class Node;


class BasePort : public MongoObject
{

protected:
    Node *node_;

public:
    
    bool is_fixed();

    bool is_output();

    bool is_reactive();

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
        bson_append_bool(&document, "is_reactive", 5, false);

        set_fixed(fixed);
        set_port_type(is_output);
        set_reactive(is_reactive);
    }

    void set_fixed(bool fixed);

    void set_port_type(bool is_output);

    void set_reactive(bool reactive);

    Node* get_node();

    void set_node(Node *node_ptr);
};

class ValuePort : public BasePort
{
private:

    std::vector<double> _buff_double_vector{};

public:

    // Constructor & Destructor
    //--------------------------------------------------------------------

    ValuePort() : BasePort()
    {
        bson_append_oid(&document, "link", 4, &oid_document);
    }

    ValuePort(double value,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false
    ) : BasePort(fixed, is_output, is_reactive)
    {
        set_value(value);
    };

    ValuePort(std::vector<double> array,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false
    ) : BasePort(fixed, is_output, is_reactive)
    {
        set_value(array.data(), array.size());
    };

    ~ValuePort(){
    };

    // Getter & Setter
    //--------------------------------------------------------------------

    virtual void set_value(double *in, int nbr_in);
    virtual void set_value(double value);
    virtual void get_value(double **out, int *nbr_out);
    virtual std::vector<double> get_value();
    bson_t get_bson() final;

    // Methods
    //--------------------------------------------------------------------

    bool write_to_db();
    bool read_from_db(const std::string &oid_string);

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
         bool is_reactive = false
    ) : ValuePort(value, fixed, is_output, is_reactive), port_links(this)
    {
        set_value(value);
    };

    Port(std::vector<double> array,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false
    ) : ValuePort(array, fixed, is_output, is_reactive), port_links(this)
    {
        set_value(array.data(), array.size());
    };

    ~Port(){
    };

    // Methods
    //--------------------------------------------------------------------

    void set_value(double *in, int nbr_in);
    void set_value(double value);
    void get_value(double **out, int *nbr_out);
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
