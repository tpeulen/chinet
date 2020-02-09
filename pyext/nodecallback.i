%module(directors="1") chinet

%include "std_map.i";
%include "std_string.i";

%{
    #include "../include/Port.h"
    #include "../include/NodeCallback.h"
%}


/* turn on director wrapping Callback */
%feature("director") NodeCallback;

%shared_ptr(Port)
%shared_ptr(NodeCallback)

%include "../include/NodeCallback.h"




