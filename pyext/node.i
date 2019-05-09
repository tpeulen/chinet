%module(directors="1") chinet

%include "std_vector.i";
%include "std_map.i";
%include "std_list.i";

%{
#include "../include/Node.h"
#include "../include/MongoObject.h"
%}

%shared_ptr(Link)
%shared_ptr(Node)

%include "../include/Node.h"
