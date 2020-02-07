#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>
#include <algorithm>
#include <math.h>

#include <CNode.h>

class Node;

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

    bool remove_links_to_port()
    {
        if (link_ != nullptr) {
            // remove pointer to this port in the port to which this is linked
            auto it = std::find(linked_to_.begin(), linked_to_.end(), this);
            if (it != linked_to_.end()) {
                linked_to_.erase(it);
                return true;
            }
        }
        return false;
    }

    void set_value_of_dependents(double *in, int nbr_in)
    {
        for(auto &v : linked_to_){
            v->set_value(in, nbr_in);
        }
    }

public:

    // Constructor & Destructor
    //--------------------------------------------------------------------

    Port() : ValuePort()
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
            is_bounded, lower_bound, upper_bound)
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
            is_bounded, lower_bound, upper_bound)
    {
    };

    ~Port(){
        remove_links_to_port();
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
        if(v != nullptr){
            link_ = v;
            v->linked_to_.push_back(this);
        }
    }

    bool unlink()
    {
        set_oid("link", get_bson_oid());

        bool re = true;
        re &= remove_links_to_port();
        // set the link to a nullptr
        link_ = nullptr;
        return re;
    }

    bool is_linked()
    {
        return (link_ != nullptr);
    }

    std::vector<Port*> get_linked_ports()
    {
        return linked_to_;
    }

    std::shared_ptr<Port> get_link()
    {
        return link_;
    }

};

#endif //chinet_PORT_H
