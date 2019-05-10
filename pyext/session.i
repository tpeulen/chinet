%module(directors="1") chinet

%include "std_set.i";

%{
#include "../include/Session.h"
%}

%shared_ptr(Session)

%template(map_string_node) std::map<std::string, std::shared_ptr<Node>>;

%include "../include/Session.h"

