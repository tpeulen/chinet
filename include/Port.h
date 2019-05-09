#ifndef chinet_PORT_H
#define chinet_PORT_H

#include <cstdint>
#include <memory>

#include <Node.h>

class Port : public MongoObject{

private:
    std::shared_ptr<Port> link_;

public:

    template <typename T>
    T get_value(){
        if(link_ == nullptr){
            return MongoObject::get_value<T>("value");
        } else{
            return link_->get_value<T>();
        }
    }

    template <typename T>
    void set_value(T v){
        MongoObject::set_value("value", v);
        if(link_ != nullptr){
            link_->set_value(v);
        }
    }

    template <typename T>
    std::vector<T> get_array(){
        if(link_ == nullptr){
            return MongoObject::get_array<T>("value");
        } else{
            return link_->get_array<T>();
        }
    }

    template <typename T>
    void set_array(std::vector<T> v){
        MongoObject::set_array("value", v);
        if(link_ != nullptr){
            link_->set_array(v);
        }
    }

    bool is_fixed();

    bool is_linked();

    void link(std::shared_ptr<Port> v);
    void unlink();

    void set_fixed(bool fixed);

    // Constructor
    //--------------------------------------------------------------------
    Port() :
    link_(nullptr)
    {
        append_string(&document, "type", "port");
        bson_append_double(&document, "value", 5, 1.0);
        bson_append_bool(&document, "fixed", 5, false);
        bson_append_oid(&document, "link", 4, &oid_document);
    };

    Port(std::vector<double> v) :
    Port()
    {
        set_array(v);
    };

    Port(double v) :
    Port()
    {
        set_value(v);
    };

    // Destructor
    //--------------------------------------------------------------------
    ~Port() = default;

    // Getter
    //--------------------------------------------------------------------
    //std::string get_name();

    // Setter
    //--------------------------------------------------------------------

    // Operator
    //--------------------------------------------------------------------
    //std::shared_ptr<Port> operator +(std::shared_ptr<Port> v);
    // std::vector<double> operator [](std::string key);

    // Methods
    //--------------------------------------------------------------------
    //virtual size_t size();

};


#endif //chinet_PORT_H
