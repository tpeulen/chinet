%module(directors="1") chinet

%{
#include "../include/Link.h"
%}

%shared_ptr(Link)

%include "../include/Link.h"
