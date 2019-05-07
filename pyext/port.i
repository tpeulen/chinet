%module(directors="1") chinet

%{
#include "../include/Port.h"
%}

%shared_ptr(Port)

%include "../include/Port.h"

