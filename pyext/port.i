%module(directors="1") chinet

%{
#include "../include/Port.h"
%}

%include "numpy.i"
%init %{
    import_array();
%}

%shared_ptr(Port)

%apply (double* IN_ARRAY1, int DIM1) {(double *in, int nbr_in)}
%apply (double** ARGOUTVIEW_ARRAY1, int* DIM1) {(double **out, int *nbr_out)}
%ignore get_value(); // use memory - void get_value(double **out, int *nbr_out)
%ignore set_value(std::vector<double> &values); // use memory - void get_value(double **out, int *nbr_out)

%include "../include/Port.h"
