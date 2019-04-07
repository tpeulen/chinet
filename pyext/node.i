%module fluomodlib

%include "std_vector.i";
%include "std_list.i";

namespace std{
        %template(VectorNode) vector<Node*>;
}

%{
#include "../include/Node.h"
#include "../include/Port.h"
#include "../include/NodeOperations.h"
#include <mongoc/mongoc.h>

%}

%shared_ptr(Node)
%shared_ptr(Port)

%include "../include/Node.h"
%include "../include/Port.h"
%include "../include/NodeOperations.h"
%include <mongoc/mongoc.h>

