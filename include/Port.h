#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>
#include <algorithm>

#include <Node.h>

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


class Port : public BasePort
{

private:

    std::vector<double> _buff_double_vector{};

protected:

    /*!
     * @brief This attribute can point to another Port (default value nullptr).
     * If the attribute points to another port, the value returned by the
     * method @class Port::get_value corresponds to the value the other Port.
     */
    std::shared_ptr<Port> link_;

    /*!
     * @brief This attribute stores the Ports that are dependent on the value
     * of this Port object. If this Port object is reactive a change of the
     * value of this Port object is propagated to the dependent Ports.
     */
    std::vector<Port*> linked_to_;

public:

    // Constructor & Destructor
    //--------------------------------------------------------------------

    Port() :
    BasePort(),
    link_(nullptr)
    {
        bson_append_oid(&document, "link", 4, &oid_document);
    }

    Port(double value,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false
    ) :
    BasePort(fixed, is_output, is_reactive),
    link_(nullptr)
    {
        set_value(value);
    };

    Port(std::vector<double> array,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false
    ) :
    BasePort(fixed, is_output, is_reactive)
    {
        set_value(array.data(), array.size());
    };

    ~Port(){
        remove_pointer_to_this_in_link_port();
    };

    // Getter & Setter
    //--------------------------------------------------------------------

    std::vector<Port*> get_linked_ports();
    std::shared_ptr<Port> get_link();
    bool is_linked();

    void set_value(double *in, int nbr_in);
    void set_value(double value);
    void get_value(double **out, int *nbr_out);
    std::vector<double> get_value();

    bson_t get_bson() final;

    // Methods
    //--------------------------------------------------------------------

    bool remove_pointer_to_this_in_link_port();

    void link(std::shared_ptr<Port> &v);

    bool unlink();

    bool write_to_db();

    bool read_from_db(const std::string &oid_string);

};


#endif //chinet_PORT_H
