%module(directors="1") chinet

%{
#include "../include/MongoObject.h"
%}

%shared_ptr(MongoObject)
%template(map_string_double_vector) std::map<std::string, std::vector<double>>;


%include "../include/MongoObject.h"

%extend MongoObject {

    public:

        %template(get_array_double) get_array<double>;
        %template(set_array_double) set_array<double>;

    std::string __repr__()
    {
        return $self->get_json();
    }
}

