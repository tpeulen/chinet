%module(directors="1") chinet

%{
#include "../include/Port.h"
%}

%shared_ptr(Port)

%include "../include/Port.h"

//%template(intPort) Port<int>;
%template(set_value_vector) Port::set_value<std::vector<double>>;
%template(set_value_double) Port::set_value<double>;
%template(set_value_int) Port::set_value<int>;
%template(set_value) Port::set_value<double>;
