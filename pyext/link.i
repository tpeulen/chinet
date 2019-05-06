%module(directors="1") chinet

%{
#include "../include/MongoObject.h"
#include "../include/Link.h"
%}

%shared_ptr(Link)

%include "../include/Link.h"
