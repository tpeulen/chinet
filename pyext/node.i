%module(directors="1", package="chinet") node

/* turn on director wrapping for Callback */
%feature("director") NodeCallback;

%include <std_vector.i>;
%include <std_map.i>;
%include <std_list.i>;
%include <std_shared_ptr.i>


%{
#include "../include/CNode.h"
#include "../include/MongoObject.h"
#include "../include/Port.h"
#include "../include/NodeCallback.h"
%}


%shared_ptr(Port)
%shared_ptr(Node)
%shared_ptr(NodeCallback)
%shared_ptr(Link)

%template(MapStringPortSharedPtr) std::map<std::string, std::shared_ptr<Port>>;
%template(ListNodePtr) std::list<std::shared_ptr<Node>>;
%template(MapStringDouble) std::map<std::string, double>;

%include "../include/CNode.h"
%include "../include/NodeCallback.h"


%extend Node {

    std::string __repr__(){
        std::ostringstream os;
        os << "Node(";
        for (auto &v : $self->get_input_ports()){
            os << (v.first)<< ",";
        }
        os << "->";
        for (auto &v : $self->get_output_ports()){
            os << (v.first)<< ",";
        }
        os << ")";
        return os.str();
    }
    %pythoncode "node_extension.py"

}

