#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>

#include <Node.h>

class Node;
class Port : public MongoObject
{

private:
    std::shared_ptr<Port> link_;
    std::vector<double> buff_double_vector_{};
    Node *node_;
    bool is_reactive_;

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
        is_reactive_ = is_reactive;
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
        is_reactive_ = is_reactive;
    };

    ~Port() = default;

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
        if (link_ == nullptr) {
            if (buff_double_vector_.empty()) {
                buff_double_vector_ = MongoObject::get_array<double>("value");
            }
            return buff_double_vector_;
        } else {
            return link_->get_value();
        }
    }

    void get_value(double **out, int *nbr_out)
    {
        if (link_ == nullptr) {
            if (buff_double_vector_.empty()) {
                buff_double_vector_ = MongoObject::get_array<double>("value");
            }
            *nbr_out = buff_double_vector_.size();
            *out = buff_double_vector_.data();
        } else {
            link_->get_value(out, nbr_out);
        }
    }

    void set_value(double *in, int nbr_in);
    void set_value(std::vector<double> &values);
    void set_value(double value);
    void set_fixed(bool fixed);
    void set_is_reactive(bool reactive){
        is_reactive_ = reactive;
    }

    bool is_fixed();
    bool is_linked();
    bool is_reactive(){
        return is_reactive_;
    }

    bson_t get_bson() final;

    // Methods
    //--------------------------------------------------------------------

    void link(std::shared_ptr<Port> &v);

    void unlink();

    bool write_to_db()
    {
        bson_t doc = get_bson();
        return MongoObject::write_to_db(doc, 0);
    }

    bool read_from_db(const std::string &oid_string)
    {
        bool re = MongoObject::read_from_db(oid_string);
        buff_double_vector_ = MongoObject::get_array<double>("value");
        return re;
    };

    bool operator==(Port &b){
        return (
                (b.get_oid() == get_oid()) ||
                (b.link_.get() == this)
                );
    };

};


#endif //chinet_PORT_H
