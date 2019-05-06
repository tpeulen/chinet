%module(directors="1") chinet

%include "std_vector.i";
%include "std_set.i";
%include "std_map.i";
%include "std_list.i";

%{
#include "../include/Port.h"
%}

%shared_ptr(Port)

%include "../include/Port.h"
