//
// Created by thomas on 5/27/19.
//

#ifndef CHINET_PORTLINKS_H
#define CHINET_PORTLINKS_H

#include <Port.h>

class Port;

template <class T>
class PortLinks
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

    Port *port_;

public:

    PortLinks(Port *port)
    {
        port_ = port;
    }

    ~PortLinks(){
        remove_links_to_port();
    };

    bool remove_links_to_port()
    {
        if(link_ != nullptr){
            // remove pointer to this port in the port to which this is linked
            auto it = std::find(link_->port_links.linked_to_.begin(),
                                link_->port_links.linked_to_.end(), port_);

            if(it != link_->port_links.linked_to_.end()){
                link_->port_links.linked_to_.erase(it);
                return true;
            }
        }
        return false;
    }

    bool is_linked()
    {
        return (link_ != nullptr);
    }

    void link(std::shared_ptr<Port> &v)
    {
        if(v != nullptr){
            link_ = v;
            v->port_links.linked_to_.push_back(port_);
        }
    }

    bool unlink()
    {
        bool re = true;
        re &= remove_links_to_port();
        // set the link to a nullptr
        link_ = nullptr;
        return re;
    }

    std::vector<Port*> get_linked_ports()
    {
        return linked_to_;
    }

    std::shared_ptr<Port> get_link()
    {
        return link_;
    }

    void set_value_of_dependents(T *in, int nbr_in)
    {
        for(auto &v : linked_to_){
            v->set_value(in, nbr_in);
        }
    }

};

#endif //CHINET_PORTLINKS_H
