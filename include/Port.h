#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>
#include <algorithm>

#include <Node.h>

class Node;

class Port : public MongoObject
{

private:

    std::vector<double> _buff_double_vector{};

protected:

    /*!
     * @brief This attribute can point to another Port (default value nullptr).
     * If the attribute points to another port, the value returned by the
     * method @class Port::get_value corresponds to the value the other Port.
     */
    std::shared_ptr<Port> _link;

    /*!
     *
     */
    std::vector<Port*> _linked_to;

    bool _is_reactive;

    Node *node_;

public:

    // Constructor & Destructor
    //--------------------------------------------------------------------

    Port() :
    node_(nullptr)
    {
        append_string(&document, "type", "port");
        bson_append_double(&document, "singleton", 9, 1.0);
        bson_append_bool(&document, "is_output", 9, false);
        bson_append_bool(&document, "fixed", 5, false);
        bson_append_oid(&document, "link", 4, &oid_document);
    }

    Port(double value,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false
    ) :
    Port()
    {
        set_value(value);
        set_fixed(fixed);
        set_port_type(is_output);
        _is_reactive = is_reactive;
    };

    Port(std::vector<double> array,
         bool fixed = false,
         bool is_output = false,
         bool is_reactive = false
    ) :
    Port()
    {
        set_fixed(fixed);
        set_port_type(is_output);
        set_value(array.data(), array.size());
        _is_reactive = is_reactive;
    };

    ~Port(){
        remove_pointer_to_this_in_link_port();
    };

    // Getter & Setter
    //--------------------------------------------------------------------

    void set_port_type(bool is_output);

    void set_node(Node *node_ptr){
        node_ = node_ptr;
    }

    Node* get_node(){
        return node_;
    }

    bool is_output()
    {
        return MongoObject::get_singleton<bool>("is_output");
    }

    std::vector<double> get_value()
    {
        if (_link == nullptr) {
            if (_buff_double_vector.empty()) {
                _buff_double_vector = MongoObject::get_array<double>("value");
            }
            return _buff_double_vector;
        } else {
            return _link->get_value();
        }
    }

    void get_value(double **out, int *nbr_out)
    {
        if (_link == nullptr) {
            if (_buff_double_vector.empty()) {
                _buff_double_vector = MongoObject::get_array<double>("value");
            }
            *nbr_out = _buff_double_vector.size();
            *out = _buff_double_vector.data();
        } else {
            _link->get_value(out, nbr_out);
        }
    }

    std::vector<Port*> get_linked_ports(){
        return _linked_to;
    }

    std::shared_ptr<Port> get_link(){
        return _link;
    }

    void set_value(double *in, int nbr_in);
    void set_value(std::vector<double> &values);
    void set_value(double value);
    void set_fixed(bool fixed);
    void set_is_reactive(bool reactive);

    bool is_fixed();
    bool is_linked();
    bool is_reactive(){
        return _is_reactive;
    }

    bson_t get_bson() final;

    // Methods
    //--------------------------------------------------------------------

    bool remove_pointer_to_this_in_link_port(){
        if(_link != nullptr){
            // remove pointer to this port in the port to which this is linked
            auto it = std::find(_link->_linked_to.begin(),
                                _link->_linked_to.end(), this);

            if(it != _link->_linked_to.end()){
                _link->_linked_to.erase(it);
                return true;
            }
        }
        return false;
    }

    void link(std::shared_ptr<Port> &v);

    bool unlink();

    bool write_to_db()
    {
        bson_t doc = get_bson();
        return MongoObject::write_to_db(doc, 0);
    }

    bool read_from_db(const std::string &oid_string)
    {
        bool re = MongoObject::read_from_db(oid_string);
        _buff_double_vector = MongoObject::get_array<double>("value");
        return re;
    }

};


#endif //chinet_PORT_H
