%module(directors="1") chinet

%{
#include "../include/MongoObject.h"
%}

%include <std_shared_ptr.i>

%shared_ptr(MongoObject)

%include attribute.i
%attributestring(MongoObject, std::string, name, get_name, set_name);
%attributestring(MongoObject, std::string, oid, get_own_oid, set_own_oid);
%attributestring(MongoObject, bool, is_connected_to_db, is_connected_to_db);

%include "../include/MongoObject.h"
%extend MongoObject {

    public:

        %template(get_array_double) get_array<double>;
        %template(set_array_double) set_array<double>;
        %template(get_array_long) get_array<long>;
        %template(set_array_long) set_array<long>;
        %template(get_array_int) get_array<int>;
        %template(set_array_int) set_array<int>;
        //%template(get_array_bool) get_array<bool>;
        //%template(set_array_bool) set_array<bool>;

        %template(get_singleton_double) get_singleton<double>;
        %template(set_singleton_double) set_singleton<double>;
        %template(get_singleton_int) get_singleton<int>;
        %template(set_singleton_int) set_singleton<int>;
        %template(get_singleton_bool) get_singleton<bool>;
        %template(set_singleton_bool) set_singleton<bool>;

        %template(connect_object_to_db_mongo) connect_object_to_db<std::shared_ptr<MongoObject>>;

    %pythoncode "mongo_extension.py"

}
