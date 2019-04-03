%module fluomodlib
%{
#include "../include/Variable.h"
%}

%include "../include/Variable.h"

%include "std_vector.i";
%include "std_list.i";
namespace std{
    %template(vector_Variable) vector<Variable*>;
}


//%template(Variable_double) Variable<double>;
