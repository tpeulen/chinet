%module chinet
%{
#include "../include/Curve.h"
%}


%apply (double* IN_ARRAY1, int DIM1) {(double *in, int n_in)}
%apply (double** ARGOUTVIEWM_ARRAY1, int* DIM1) {(double **out, int *n_out)}

%ignore Curve::get_x();
%ignore Curve::get_y();

%include "../include/Curve.h"

