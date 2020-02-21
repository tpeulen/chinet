%module(directors="1") chinet
%feature("kwargs", 1);

%{
#include "../include/Port.h"
#include "../include/CNode.h"
%}

%include "./include/numpy.i"
%include "std_vector.i";

%include <std_shared_ptr.i>
%shared_ptr(Port)
%shared_ptr(Node)

%init %{
    import_array();
%}

%apply (double* IN_ARRAY1, int DIM1) {(double *input, int n_input)}
%apply (double** ARGOUTVIEW_ARRAY1, int* DIM1) {(double **output, int *n_output)}
%apply (long* IN_ARRAY1, int DIM1) {(long *input, int n_input)}
%apply (long** ARGOUTVIEW_ARRAY1, int* DIM1) {(long **output, int *n_output)}
%apply (unsigned char* IN_ARRAY1, int DIM1) {(unsigned char *input, int n_input)}
%apply (unsigned char** ARGOUTVIEW_ARRAY1, int* DIM1) {(unsigned char **output, int *n_output)}

%template(vector_port_ptr) std::vector<Port*>;

%include attribute.i
%attribute(Port, bool, fixed, is_fixed, set_fixed);
%attribute(Port, bool, is_output, is_output, set_port_type);
%attribute(Port, bool, reactive, is_reactive, set_reactive);
%attribute(Port, bool, bounded, is_bounded, set_bounded);
%attributestring(Port, std::string, name, get_name, set_name);
%attribute(Port, Port*, link, get_link, set_link);

%include "../include/Port.h"
%extend Port{

    public:
        %template(set_value_d) set_value<double>;
        %template(get_value_d) get_value<double>;
        %template(get_value_i) get_value<long>;
        %template(set_value_i) set_value<long>;
        %template(get_value_c) get_value<unsigned char>;
        %template(set_value_c) set_value<unsigned char>;

    std::string __repr__(){
        std::ostringstream os;
        os << $self->get_json();
        os << std::endl;
        return os.str();
    }

    %pythoncode
    %{
    __swig_getmethods__["node"] = get_node
    __swig_setmethods__["node"] = set_node
    __swig_getmethods__["bytes"] = get_bytes
    __swig_setmethods__["bytes"] = set_bytes
    __swig_getmethods__["bounds"] = get_bounds
    __swig_setmethods__["bounds"] = set_bounds

    @property
    def value(self):
        if self.value_type == 0:
            return self.get_value_i()
        else:
            return self.get_value_d()

    @value.setter
    def value(self, v):
        if v.dtype.kind == 'i':
            self.set_value_i(v, True)
        else:
            self.set_value_d(v, True)

    def __init__(self, value = [], *args, **kwargs):
        import numpy as np
        this = _chinet.new_Port(*args, **kwargs)
        try:
            self.this.append(this)
        except:
            self.this = this
        if not isinstance(value, np.ndarray):
            value = np.atleast_1d(value)
        if value.dtype.kind == 'i':
            self.set_value_i(value)
        else:
            self.set_value_d(value)
    %}
}
