%module(directors="1") fluomodlib

%include "std_vector.i";
%include "std_list.i";


namespace std{
%template(VectorNode) vector<Node*>;
}


%{
#include "../include/Node.h"
#include "../include/Port.h"
#include "../include/NodeCallback.h"

%}

/* turn on director wrapping Callback */
%feature("director") NodeCallback;

%shared_ptr(Node)
%shared_ptr(NodeCallback)
%shared_ptr(Port)


%feature("pythonprepend") Node::set_callback(std::shared_ptr<NodeCallback>) %{
    # adding the callback to Node object: ensure that the PyObject is correctly deleted
    self.call = cb
%}


%include "../include/Node.h"
%include "../include/Port.h"
%include "../include/NodeCallback.h"


