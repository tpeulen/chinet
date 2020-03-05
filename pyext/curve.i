%module chinet
%{
#include "../include/_old/Curve.h"

%}


%apply (double* IN_ARRAY1, int DIM1) {(double *input, int n_input)}
%apply (double** ARGOUTVIEWM_ARRAY1, int* DIM1) {(double **output, int *n_output)}

%ignore Curve::get_x();
%ignore Curve::get_y();

%include "../include/Curve.h"

