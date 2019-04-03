%module fluomodlib

%include "std_vector.i";
%include "std_list.i";
namespace std{
        %template(VectorNode) vector<Node*>;
}

%{
#include "../include/Node.h"
#include "../include/NodeOperations.h"

%}

%include "../include/Node.h"
%include "../include/NodeOperations.h"
