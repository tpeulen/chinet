%module fluomodlib
%{
#include "../include/Curve.h"
%}

%import std_vector.i

%apply (double* IN_ARRAY1, int DIM1) {(double *in, int n_in)}
%apply (double** ARGOUTVIEWM_ARRAY1, int* DIM1) {(double **out, int *n_out)}

%include "../include/Curve.h"

