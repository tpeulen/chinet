%module chinet

%include "std_vector.i";
%include "std_map.i";
%include "std_list.i";


%{
#include "../include/CNode.h"
#include "../include/MongoObject.h"
#include "../include/Port.h"
%}

%shared_ptr(Node)
%shared_ptr(Port)

%template(MapStringPort) std::map<std::string, std::shared_ptr<Port>>;
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

    /*
    std::shared_ptr<Port> __getitem__(std::string key)
    {
            return (*self)[key];
    };
     */

}

