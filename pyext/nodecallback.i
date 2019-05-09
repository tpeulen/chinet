%module(directors="1") chinet

%include "std_map.i";
%include "std_string.i";

%{
#include "../include/Port.h"
#include "../include/NodeCallback.h"
%}

namespace std{
        %template(map_string_port) map<string, shared_ptr<Port>>;
}

/* turn on director wrapping Callback */
%feature("director") NodeCallback;

%shared_ptr(Port)
%shared_ptr(NodeCallback)

%include "../include/NodeCallback.h"




