#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>

#include <Node.h>

class Port : public MongoObject {

private:
    std::shared_ptr<Port> link_;
    std::vector<double> buff_double_vector_{1};

public:

    // Constructor & Destructor
    //--------------------------------------------------------------------
    Port() :
            link_(nullptr) {
        append_string(&document, "type", "port");
        bson_append_double(&document, "singleton", 9, 1.0);
        bson_append_bool(&document, "fixed", 5, false);
        bson_append_oid(&document, "link", 4, &oid_document);
    };

    Port(std::vector<double> array, bool fix = false, std::shared_ptr<Port> linked_port = nullptr) :
            Port() {
        set_array(array);
        set_fixed(fix);
        link(linked_port);
    };

    Port(double value, bool fix = false, std::shared_ptr<Port> linked_port = nullptr) :
            Port() {
        set_value(value);
        set_fixed(fix);
        link(linked_port);
    };

    ~Port() = default;

    // Getter & Setter
    //--------------------------------------------------------------------

    template<typename T>
    T get_value() {
        if (link_ == nullptr) {
            return MongoObject::get_value<T>("singleton");
        } else {
            return link_->get_value<T>();
        }
    }

    template<typename T>
    void set_value(T v) {
        MongoObject::set_value("singleton", v);
        if (link_ != nullptr) {
            link_->set_value(v);
        }
    }

    template<typename T>
    T get_array() {
        if (link_ == nullptr) {
            if (buff_double_vector_.empty()) {
                buff_double_vector_ = MongoObject::get_array<double>("vector");
            }
            return buff_double_vector_;
        } else {
            return link_->get_array<T>();
        }
    }

    template<typename T>
    void set_array(T value) {
        buff_double_vector_ = value;
        if (link_ != nullptr) {
            link_->set_array(value);
        }
    }

    void set_fixed(bool fixed);

    bool is_fixed();

    bool is_linked();

    bson_t get_bson() final;

    // Methods
    //--------------------------------------------------------------------

    void link(std::shared_ptr<Port> v);

    void unlink();

    bool write_to_db() {
        bson_t doc = get_bson();
        return MongoObject::write_to_db(doc, 0);
    }

    bool read_from_db(const std::string &oid_string) {
        bool re = MongoObject::read_from_db(oid_string);
        buff_double_vector_ = MongoObject::get_array<double>("vector");
        return re;
    };


};


#endif //chinet_PORT_H
