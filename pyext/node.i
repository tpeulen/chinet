%module(directors="1") chinet

%include "std_vector.i";
%include "std_map.i";
%include "std_list.i";

%{
#include "../include/Node.h"
%}

%shared_ptr(Value)
%shared_ptr(Link)
%shared_ptr(Node)

%include "../include/Node.h"

namespace std{
        %template(map_string_port) map<string, shared_ptr<Port>>;
}
