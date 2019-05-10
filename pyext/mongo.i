%module(directors="1") chinet

%{
#include "../include/MongoObject.h"
%}

%shared_ptr(MongoObject)

%template(map_string_double_vector) std::map<std::string, std::vector<double>>;

%extend MongoObject {
    std::string __repr__()
    {
        return $self->get_json();
    }
}

%include "../include/MongoObject.h"
