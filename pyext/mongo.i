%module(directors="1") chinet

%{
#include "../include/MongoObject.h"
%}

%shared_ptr(MongoObject)

%include "../include/MongoObject.h"

namespace std{
        %template(map_string_double) std::map<std::string, std::vector<double>>;
}

