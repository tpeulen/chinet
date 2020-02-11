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
        %template(get_array_int) get_array<long>;
        %template(set_array_int) set_array<long>;
        //%template(get_array_bool) get_array<bool>;
        //%template(set_array_bool) set_array<bool>;

        %template(get_singleton_double) get_singleton<double>;
        %template(set_singleton_double) set_singleton<double>;
        %template(get_singleton_int) get_singleton<long>;
        %template(set_singleton_int) set_singleton<long>;
        %template(get_singleton_bool) get_singleton<bool>;
        %template(set_singleton_bool) set_singleton<bool>;

        %template(connect_object_to_db_mongo) connect_object_to_db<std::shared_ptr<MongoObject>>;

    std::string __repr__()
    {
        return $self->get_json();
    }

    std::shared_ptr<MongoObject> __getitem__(std::string key){
        return (*self)[key];
    };

}
