#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <bson.h>

#include "CNode.h"
#include "MongoObject.h"

class Node;

class Port : public MongoObject {

private:
    std::vector<uint8_t> buffer_;
    int buffer_element_size_ = 1;
    std::vector<double> bounds_{};
    Node* node_ = nullptr;

    std::shared_ptr<Port> link_ = nullptr;
    std::vector<Port *> linked_to_;

    bool remove_links_to_port() {
        if (link_ == nullptr) return false;
        auto& linked_ports = link_->linked_to_;
        auto it = std::find(linked_ports.begin(), linked_ports.end(), this);
        if (it != linked_ports.end()) {
            linked_ports.erase(it);
            return true;
        }
        return false;
    }

    template<typename T>
    void set_value_of_dependents(T *input, int n_input) {
        for (auto &v : linked_to_) {
            v->set_value(input, n_input);
        }
    }

    int value_type = 0;
    bool fixed_ = false;
    bool is_output_ = false;
    bool is_reactive_ = false;
    bool is_bounded_ = false;

public:
    size_t current_size() {
        return buffer_.size() / buffer_element_size_;
    }

    virtual std::shared_ptr<Port> get_ptr();
    ~Port() { remove_links_to_port(); }

    Port(
            bool fixed = false,
            bool is_output = false,
            bool is_reactive = false,
            bool is_bounded = false,
            double lb = 0,
            double ub = 0,
            int value_type = 1,
            std::string name = ""
    ) : MongoObject(name), fixed_(fixed), is_output_(is_output), is_reactive_(is_reactive), is_bounded_(is_bounded), value_type(value_type) {
        append_string(&document, "type", "port");
        buffer_.resize(1);
        if (is_bounded) {
            bounds_.push_back(lb);
            bounds_.push_back(ub);
        }
    }

    void set_fixed(bool fixed) { fixed_ = fixed; }
    bool is_fixed() const { return fixed_; }

    void set_port_type(bool is_output) { is_output_ = is_output; }
    bool is_output() const { return is_output_; }

    void set_reactive(bool is_reactive) { is_reactive_ = is_reactive; }
    bool is_reactive() const { return is_reactive_; }

    void set_bounded(bool is_bounded) { is_bounded_ = is_bounded; }
    bool is_bounded() const { return is_bounded_; }

    void set_value_type(int type) { value_type = type; }
    int get_value_type() const { return value_type; }

    void set_node(Node* node_ptr) { node_ = node_ptr; }
    Node* get_node() const { return node_; }

    void set_link(std::shared_ptr<Port> v);
    bool is_linked() const { return link_ != nullptr; }
    std::shared_ptr<Port> get_link() { return link_; }

    bool unlink() {
        if (link_ == nullptr) return false;
        set_oid("link", get_bson_oid());
        bool result = remove_links_to_port();
        link_ = nullptr;
        return result;
    }

    bool bound_is_valid();
    void set_bounds(double *input, int n_input);
    void get_bounds(double **output, int *n_output);

    bool is_float() {
        return ((get_value_type() == 1) || (get_value_type() == 3));
    }

    void get_bytes(unsigned char **output, int *n_output, bool copy = false);

    void set_bytes(unsigned char *input, int n_input);

    void set_buffer_ptr(size_t ptr, int n_elements, int element_size);

    size_t get_buffer_ptr();

    std::vector<Port *> get_linked_ports();

    template<typename T>
    void set_value(T *input, int n_input, bool copy_values = true) {
        if (is_fixed()) {
            return;
        }
        if (copy_values) {
            buffer_.resize(n_input * sizeof(T));
            std::memcpy(buffer_.data(), input, n_input * sizeof(T));
        } else {
            buffer_ = std::vector<uint8_t>(reinterpret_cast<uint8_t*>(input),
                                           reinterpret_cast<uint8_t*>(input) + n_input * sizeof(T));
        }
        buffer_element_size_ = sizeof(T);
        if (node_ != nullptr) {
            update_attached_node();
            set_value_of_dependents(input, n_input);
        }
    }

    template<typename T>
    void get_value(T **output, int *n_output) {
        *n_output = buffer_.size() / sizeof(T);
        *output = reinterpret_cast<T*>(buffer_.data());
    }

    template<typename T>
    void get_own_value(T **output, int *n_output) {
        if (buffer_.empty()) {
            update_buffer<T>();
        }
        *n_output = buffer_.size() / sizeof(T);
        *output = reinterpret_cast<T*>(buffer_.data());
    }

    virtual bson_t get_bson() final;

    template<typename T>
    void update_buffer() {
        auto v = get_array<T>("value");
        buffer_.resize(v.size() * sizeof(T));
        std::memcpy(buffer_.data(), v.data(), v.size() * sizeof(T));
    }

    std::vector<uint8_t>& get_buffer() { return buffer_; }

    void update_attached_node();

    bool write_to_db();
    bool read_from_db(const std::string &oid_string);

    std::shared_ptr<Port> operator+(std::shared_ptr<Port> v);
    std::shared_ptr<Port> operator*(std::shared_ptr<Port> v);
};

#endif //chinet_PORT_H
