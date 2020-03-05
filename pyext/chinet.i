%module(directors="1", package="chinet") chinet
%feature("kwargs", 1);
%warnfilter(511) std::vector;
%warnfilter(511) std::map;
%warnfilter(511) swig::PySwigIterator;
%include "documentation.i"
%{
#define SWIG_FILE_WITH_INIT
#include "../include/Functions.h"
#include "../include/CNode.h"
#include "../include/MongoObject.h"
%}

%pythonbegin %{
import numpy as np
import chinet as cn
import inspect
import typing
%}

%include "typemaps.i";
%include "stl.i";
%include "std_wstring.i";
%include "std_string.i";
%include "std_map.i";
%include "std_vector.i";
%include "std_list.i";
%include "std_shared_ptr.i";
%include "cpointer.i"

%template(MapStringString) std::map<std::string, std::string>;
%template(VectorString) std::vector<std::string>;
%template(VectorDouble) std::vector<double>;
%template(VectorInt) std::vector<int>;
%template(VectorLong) std::vector<long>;

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
%include "session.i"
//%include "pda.i"
