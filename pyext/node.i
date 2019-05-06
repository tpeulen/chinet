%module(directors="1") chinet

%include "std_vector.i";
%include "std_map.i";
%include "std_list.i";


%{
#include "../include/MongoObject.h"
#include "../include/Node.h"
#include "../include/NodeCallback.h"
%}

/* turn on director wrapping Callback */
%feature("director") NodeCallback;

%shared_ptr(Value)
%shared_ptr(Link)
%shared_ptr(Node)
%shared_ptr(NodeCallback)

%include "../include/Node.h"
%include "../include/NodeCallback.h"
%include "../include/MongoObject.h"

namespace std{
        %template(VectorNode) vector<Node*>;
        %template(map_string_vector_double) map<string, vector<double>>;
        %template(map_string_port) map<string, shared_ptr<Port>>;
}
