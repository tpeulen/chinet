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

%template(map_string_string) std::map<std::string, std::string>;
%template(vector_string) std::vector<std::string>;

// Check if this helps
%template(DoubleVector) std::vector<double>;
%extend vector<double> {
    string __str__(){
        std::ostringstream os;
        os << "Double Vector: [ ";
        int len = $self->size();
        if (len > 100){ // only print the first and last few
            for (int i=0 ; i < 5; i++){
                os << $self->at(i) << ", ";
            }
            os << " ..... ";
            for (int i=len-5 ; i < len; i++){
                os << $self->at(i) << ", ";
            }
        }
        else{
            for (int i=0 ; i < len; i++){
                os << $self->at(i) << ", ";
            }
        }
        os << "]";
        return os.str();
    }
}
%extend vector<double> {
    string __repr__(){
        std::ostringstream os;
        os << "DoubleVector( [";
        for (int i=0 ; i < $self->size(); i++){
            os << $self->at(i) << ", ";
        }
        os << "] )";
        return os.str();
    }
}


%include "mongo.i"
%include "port.i"
%include "node.i"
%include "nodecallback.i"
%include "session.i"
%include "cpointer.i"

/*
%include "curve.i"
%include "decay.i"
%include "pda.i"
*/


