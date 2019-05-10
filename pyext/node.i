%module chinet

%include "std_vector.i";
%include "std_map.i";
%include "std_list.i";

%{
#include "../include/Node.h"
#include "../include/MongoObject.h"
#include "../include/Port.h"
%}

%shared_ptr(Node)
%shared_ptr(Port)

namespace std{
    %template(map_string_port) map<string, shared_ptr<Port>>;
    %template(map_string_double) map<string, double>;
}

%include "../include/Node.h"
