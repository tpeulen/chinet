%module(directors="1") chinet

%include "documentation.i"

%{
#define SWIG_FILE_WITH_INIT
    /*
#include "../include/Curve.h"
#include "../include/Functions.h"
#include "../include/Decay.h"
#include "../include/Node.h"
#include "../include/Pda.h"
#include "../include/MongoObject.h"
     */
%}

%include <typemaps.i>
%include "stl.i"
%include "std_wstring.i"
%include std_string.i
%include "std_map.i"
%include "std_vector.i";
%include "std_list.i";
%include "std_shared_ptr.i"

%include "numpy.i"
%init %{
    import_array();
%}

namespace std{
    %template(map_string_string) map<string, string>;
    %template(vector_double) vector<double>;
    %template(vector_string) vector<string>;
}

%include "mongo.i"
%include "port.i"
%include "node.i"
%include "nodecallback.i"
%include "session.i"

/*
%include "curve.i"
%include "decay.i"
%include "pda.i"
*/


