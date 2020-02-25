%module chinet

%include "std_vector.i";
%include "std_map.i";
%include "std_list.i";


%{
#include "../include/CNode.h"
#include "../include/MongoObject.h"
#include "../include/Port.h"
%}

%shared_ptr(Port)
%shared_ptr(Node)
%template(MapStringSharedPort) std::map<std::string, std::shared_ptr<Port>>;
%template(MapStringDouble) std::map<std::string, double>;

%include "../include/CNode.h"


%extend Node {

    std::string __repr__(){
        std::ostringstream os;
        os << "Inputs: [ ";
        for (auto &v : $self->get_input_ports()){
            os << (v.first)<< ", ";
        }
        os << "]\n";

        os << "Outputs: [ ";
        for (auto &v : $self->get_output_ports()){
            os << (v.first)<< ", ";
        }
        os << "]";
        return os.str();
    }
    %pythoncode
    %{
        __swig_getmethods__["valid"] = is_valid
        __swig_getmethods__["valid"] = set_valid
        if _newclass: valid = property(is_valid, set_valid)

        __swig_getmethods__["inputs"] = get_input_ports
        __swig_getmethods__["inputs"] = None
        if _newclass: inputs = property(get_input_ports, None)

        __swig_getmethods__["outputs"] = get_output_ports
        __swig_getmethods__["outputs"] = None
        if _newclass: outputs = property(get_output_ports, None)

        __swig_getmethods__["ports"] = get_ports
        __swig_getmethods__["ports"] = None
        if _newclass: ports = property(get_ports, None)

     %}

    /*
    std::shared_ptr<Port> __getitem__(std::string key)
    {
            return (*self)[key];
    };
     */

}

