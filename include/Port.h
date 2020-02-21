#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>
#include <algorithm>
#include <cmath>
#include <cstdlib>

#include <CNode.h>

class Node;


class Port : public MongoObject
{

private:

    void* buffer_;
    bool own_buffer = false;
    int n_buffer_elements_ = 0;
    int buffer_element_size_ = 1;
    std::vector<double> bounds_{};
    Node* node_ = nullptr;

    /*!
     * @brief This attribute can point to another Port (default value nullptr).
     * If the attribute points to another port, the value returned by the
     * method @class Port::get_value_vector corresponds to the value the other Port.
     */
    Port* link_ = nullptr;

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

    template <typename T>
    void set_value_of_dependents(T *input, int n_input)
    {
        for(auto &v : linked_to_){
            v->set_value(input, n_input);
        }
    }

public:
    int value_type = 0; // 0 long vector, 1 double vector, 2 numpy binary
    size_t current_size(){
        return n_buffer_elements_;
    }

    // Constructor & Destructor
    //--------------------------------------------------------------------
    ~Port(){
        remove_links_to_port();
        if(own_buffer){
            free(buffer_);
        }
    };

    Port(
            int value_type = 1,
            bool fixed = false,
            bool is_output = false,
            bool is_reactive = false,
            bool is_bounded = false,
            double lb = 0,
            double ub = 0,
            std::string name = ""
    ) : MongoObject(name)
    {
        append_string(&document, "type", "port");
        bson_append_bool(&document, "is_output", 9, false);
        bson_append_bool(&document, "is_fixed", 8, false);
        bson_append_bool(&document, "is_reactive", 11, false);
        bson_append_oid(&document, "link", 4, &oid_document);
        bson_append_bool(&document, "is_bounded", 10, false);
        buffer_ = malloc(1);

        set_fixed(fixed);
        set_port_type(is_output);
        set_reactive(is_reactive);
        set_bounded(is_bounded);
        if(is_bounded){
            bounds_.push_back(lb);
            bounds_.push_back(ub);
        }
        Port::value_type = value_type;
    }

    void set_node(Node* node_ptr);
    Node* get_node();

    // Getter & Setter
    //--------------------------------------------------------------------
    int get_value_type(){
        if(!is_linked()){
            return value_type;
        } else{
            return get_link()->value_type;
        }
    }

    void set_value_type(int v){
        value_type = v;
        if(is_linked()){
            get_link()->set_value_type(v);
        }
    }

    template <typename T>
    void set_value(
            T *input,
            int n_input,
            bool copy_values = true
            )
    {
        own_buffer = copy_values;
        buffer_element_size_ = sizeof(T);
        n_buffer_elements_ = n_input;
        if(std::is_same<T, double>::value){
            value_type = 1;
        } else{
            value_type = 0;
        }
        if(is_fixed()){
#if DEBUG
            std::clog << "Change value of fixed port - action ignored." << std::endl;
#endif
            return;
        }
#if DEBUG
        std::clog << "Number of elements: " << n_input << std::endl;
        std::clog << "Port:" << get_name() << ".set_value" << std::endl;
#endif
        if(copy_values){
            if(n_input > n_buffer_elements_)
                buffer_ = std::realloc(buffer_, n_input * sizeof(T));
            if(buffer_ != nullptr){
                memcpy(buffer_, input, n_input * sizeof(T));
            }
        } else{
            buffer_ = input;
        }
        if (node_ != nullptr) {
#if DEBUG
            std::clog << "Port is attached to node:" << node_->get_name() << std::endl;
#endif
            update_attached_node();
        }
    }

    template <typename T>
    void update_buffer(){
        auto v = get_array<T>("value");
        n_buffer_elements_ = v.size();
        buffer_ = realloc(buffer_, buffer_element_size_ * n_buffer_elements_);
        memcpy(buffer_, v.data(), n_buffer_elements_ * sizeof(T));
    }

    template <typename T>
    void get_own_value(T **output, int *n_output){
        if (n_buffer_elements_ == 0) {
#if DEBUG
            std::clog << "Updating buffer from bson" << std::endl;
#endif
            update_buffer<T>();
        }
        if (is_bounded() && bound_is_valid()) {
#if DEBUG
            std::clog << "bound values to: (" << bounds_[0] << ", " << bounds_[1] << ")" << std::endl;
#endif
            auto bounded_array = (T*) malloc(n_buffer_elements_ * sizeof(T));
            memcpy(bounded_array, buffer_, (size_t) n_buffer_elements_ * sizeof(T));
            Functions::map_to_bounds<T>(
                    bounded_array, n_buffer_elements_,
                    bounds_[0], bounds_[1]
                    );
            *n_output = n_buffer_elements_;
            *output = reinterpret_cast<T*>(bounded_array);
        } else {
            *n_output = n_buffer_elements_;
            *output = reinterpret_cast<T*>(buffer_);
        }
    }

    template <typename T>
    void get_value(T **output, int *n_output)
    {
        if (!is_linked()) {
            get_own_value(output, n_output);
        } else {
            get_link()->get_value(output, n_output);
        }
    }

    virtual bson_t get_bson() final;

    // Methods
    //--------------------------------------------------------------------
    void update_attached_node();
    void set_bounded(bool bounded);
    bool bound_is_valid();
    bool is_bounded();
    void set_bounds(double *input, int n_input);
    void get_bounds(double **output, int *n_output);
    bool is_fixed();
    void set_fixed(bool fixed);
    bool is_output();
    void set_port_type(bool is_output);
    bool is_reactive();
    bool is_float(){
        return (get_value_type() == 1);
    }
    void set_reactive(bool reactive);
    bool write_to_db();
    bool read_from_db(const std::string &oid_string);
    void get_bytes(unsigned char **output, int *n_output, bool copy=false);
    void set_bytes(unsigned char *input, int n_input);
    void set_buffer_ptr(size_t ptr, int n_elements, int element_size){
        buffer_ = (void*) ptr;
        buffer_element_size_ = element_size;
        n_buffer_elements_ = n_elements;
    }
    size_t get_buffer_ptr(){
        return (size_t)(&buffer_);
    }

    void set_link(Port* v)
    {
        if(v != nullptr){
            set_oid("link", v->get_bson_oid());
            link_ = v;
            v->linked_to_.push_back(this);
        } else{
            unlink();
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

    Port* get_link()
    {
        return link_;
    }

    // Operators
    //---------------------------------------

    Port* operator+(Port* v);
    Port* operator*(Port* v);
//    Port operator-(Port &v);
//    Port operator/(Port &v);

};

#endif //chinet_PORT_H
