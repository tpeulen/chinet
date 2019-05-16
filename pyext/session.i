%module(directors="1") chinet

%include "std_set.i";

%{
#include "../include/Session.h"
%}

%template(MapStringNode) std::map<std::string, std::shared_ptr<Node>>;
%shared_ptr(Session)

%include "../include/Session.h"


