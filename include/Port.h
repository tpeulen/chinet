#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>
#include <algorithm>
#include <math.h>
#include <cstdlib>

#include <CNode.h>

class Node;


class Port : public MongoObject
{

private:

    void* buffer_;
    std::vector<double> bounds_{};
    size_t n_buffer = 0;

    Node* node_ = nullptr;

    /*!
     * @brief This attribute can point to another Port (default value nullptr).
     * If the attribute points to another port, the value returned by the
     * method @class Port::get_value_vector corresponds to the value the other Port.
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
    ~Port(){
        remove_links_to_port();
        free(buffer_);
    };

    Port()
    {
        append_string(&document, "type", "port");
        bson_append_bool(&document, "is_output", 9, false);
        bson_append_bool(&document, "is_fixed", 8, false);
        bson_append_bool(&document, "is_reactive", 11, false);
        bson_append_oid(&document, "link", 4, &oid_document);
        bson_append_bool(&document, "is_bounded", 10, false);
        buffer_ = malloc(1);
    }

    Port(
            std::vector<double> array,
            bool fixed = false,
            bool is_output = false,
            bool is_reactive = false,
            bool is_bounded = false,
            double lower_bound = 0,
            double upper_bound = 0
    ) : Port()
    {
        set_fixed(fixed);
        set_port_type(is_output);
        set_reactive(is_reactive);
        set_bounded(is_bounded);
        if(is_bounded){
            set_bounds(lower_bound, upper_bound);
        }
        set_value(array.data(), array.size());
    };

    void set_node(Node *node_ptr);
    Node* get_node();

    // Getter & Setter
    //--------------------------------------------------------------------
    template <typename T> void set_value(T *in, int n_in);
    template <typename T> void get_value(T **out, int *n_out);

    template <typename T> void
    set_value_vector(std::vector<T> value)
    {
        auto v = std::vector<T>{value};
        set_value(v.data(), v.size());
    }

    template <typename T>
    std::vector<T> get_value_vector()
    {
        T *out;
        int n_out;
        get_value(&out, &n_out);

        std::vector<T> v{};
        v.assign(out, out + n_out);
        return v;
    }

    virtual bson_t get_bson() final;

    // Methods
    //--------------------------------------------------------------------
    void set_bounded(bool bounded);
    bool bound_is_valid();
    bool is_bounded();
    void set_bounds(double lower, double upper);
    void set_bounds(std::vector<double> bound);
    std::vector<double> get_bounds();

    bool is_fixed();
    void set_fixed(bool fixed);

    bool is_output();
    void set_port_type(bool is_output);

    bool is_reactive();
    void set_reactive(bool reactive);

    bool write_to_db();

    bool read_from_db(const std::string &oid_string);

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

    // Operators
    //--------------------------------------------------------------------
    Port operator+(Port &v);
    Port operator-(Port &v);
    Port operator*(Port &v);
    Port operator/(Port &v);

};

#endif //chinet_PORT_H
