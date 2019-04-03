%module fluomodlib
%include "documentation.i"

%{
#define SWIG_FILE_WITH_INIT
#include "../include/Curve.h"
#include "../include/Functions.h"
#include "../include/Decay.h"
#include "../include/Node.h"
#include "../include/Pda.h"

%}

%include <typemaps.i>
%include "stl.i"
%include "std_wstring.i"
%include std_string.i
%include "std_map.i"
%include "std_vector.i";
%include "std_list.i";

%include "numpy.i"
%init %{
import_array();
%}

namespace std{
        %template(map_string_string) map<string, string>;
        %template(vector_double) vector<double>;
}

%include "curve.i"
%include "decay.i"
%include "node.i"
%include "pda.i"

