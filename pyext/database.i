%module(directors="1") chinet

%{
#include "../include/DatabaseObject.h"
#ifdef WITH_MONGODB
#include "../include/MongoObject.h"
#else
#include "../include/MemoryObject.h"
#endif
%}

// Add typemaps to handle None -> nullptr conversion for shared_ptr parameters
#ifdef WITH_MONGODB
%typemap(in) std::shared_ptr<MongoObject> {
    if ($input == Py_None) {
        $1 = nullptr;
    } else {
        void *argp = 0;
        int res = SWIG_ConvertPtr($input, &argp, SWIGTYPE_p_std__shared_ptrT_MongoObject_t, 0);
        if (!SWIG_IsOK(res)) {
            SWIG_exception_fail(SWIG_ArgError(res), "in method '$symname', argument $argnum of type 'std::shared_ptr<MongoObject>'");
        }
        if (argp) {
            $1 = *reinterpret_cast<std::shared_ptr<MongoObject>*>(argp);
        }
    }
}
#else
%typemap(in) std::shared_ptr<MemoryObject> {
    if ($input == Py_None) {
        $1 = nullptr;
    } else {
        void *argp = 0;
        int res = SWIG_ConvertPtr($input, &argp, SWIGTYPE_p_std__shared_ptrT_MemoryObject_t, 0);
        if (!SWIG_IsOK(res)) {
            SWIG_exception_fail(SWIG_ArgError(res), "in method '$symname', argument $argnum of type 'std::shared_ptr<MemoryObject>'");
        }
        if (argp) {
            $1 = *reinterpret_cast<std::shared_ptr<MemoryObject>*>(argp);
        }
    }
}
#endif

#ifdef WITH_MONGODB
%include "../include/MongoObject.h"
#else
%include "../include/MemoryObject.h"
#endif
%include "../include/DatabaseObject.h"

// Explicitly create a Python class for DatabaseObject
#ifdef WITH_MONGODB
%pythoncode %{
DatabaseObject = MongoObject
%}
#else
%pythoncode %{
DatabaseObject = MemoryObject
%}
#endif

#ifdef WITH_MONGODB
// When MongoDB is enabled, DatabaseObject is a typedef for MongoObject
%shared_ptr(MongoObject)
%template(ListDatabaseObjectPtr) std::list<std::shared_ptr<MongoObject>>;
%attributestring(MongoObject, std::string, name, get_name, set_name);
%attributestring(MongoObject, std::string, oid, get_own_oid, set_own_oid);
%attribute(MongoObject, bool, is_connected_to_db, is_connected_to_db);

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

        %template(connect_object_to_db) connect_object_to_db<std::shared_ptr<MongoObject>>;

        std::shared_ptr<MongoObject> __getitem__(char* key) {
            return(*self)[key];
        }

    %pythoncode "database_extension.py"
}
#else
// When MongoDB is disabled, DatabaseObject is a typedef for MemoryObject
%shared_ptr(MemoryObject)
%template(ListDatabaseObjectPtr) std::list<std::shared_ptr<MemoryObject>>;
%attributestring(MemoryObject, std::string, name, get_name, set_name);
%attributestring(MemoryObject, std::string, oid, get_own_oid, set_own_oid);
%attribute(MemoryObject, bool, is_connected_to_db, is_connected_to_db);

%extend MemoryObject {
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

        %template(connect_object_to_db) connect_object_to_db<std::shared_ptr<MemoryObject>>;

        std::shared_ptr<MemoryObject> __getitem__(char* key) {
            return(*self)[key];
        }

    %pythoncode "database_extension.py"
}
#endif
