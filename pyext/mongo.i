%module(directors="1") chinet


%{
#include "../include/MongoObject.h"
%}

%shared_ptr(MongoObject)

%include "../include/MongoObject.h"
