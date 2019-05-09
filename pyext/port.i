%module(directors="1") chinet

%{
#include "../include/Port.h"
%}

%shared_ptr(Port)

%include "../include/Port.h"

//%template(intPort) Port<int>;
%template(get_value) Port::get_value<double>;
%template(set_value) Port::set_value<double>;
%template(get_array) Port::get_array<std::vector<double>>;
%template(set_array) Port::set_array<std::vector<double>>;

