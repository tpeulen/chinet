%module(directors="1") chinet

%include "std_vector.i";
%include "std_map.i";
%include "std_list.i";


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

%include "../include/Node.h"
%include "../include/Port.h"
%include "../include/NodeCallback.h"

namespace std{
        %template(VectorNode) vector<Node*>;
        %template(map_string_vector) map<string, vector<double>>;
}
