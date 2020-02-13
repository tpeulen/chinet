%module(directors="1") chinet

%{
#include "../include/Port.h"
#include "../include/CNode.h"
%}

%include "./include/numpy.i"
%include <std_shared_ptr.i>
%shared_ptr(Port)
%shared_ptr(Node)

%init %{
    import_array();
%}
%apply (double* INPLACE_ARRAY1, int DIM1) {(double *input, int n_input)}
%apply (double** ARGOUTVIEW_ARRAY1, int* DIM1) {(double **output, int *n_output)}
%apply (long* INPLACE_ARRAY1, int DIM1) {(long *input, int n_input)}
%apply (long** ARGOUTVIEW_ARRAY1, int* DIM1) {(long **output, int *n_output)}

%template(vector_port_ptr) std::vector<Port*>;
%include attribute.i
%attribute(Port, bool, fixed, is_fixed, set_fixed);
%attribute(Port, bool, is_output, is_output, set_port_type);
%attribute(Port, bool, reactive, is_reactive, set_reactive);
%attribute(Port, bool, bounded, is_bounded, set_bounded);
%attributestring(Port, std::string, name, get_name, set_name);
%attributestring(Port, Port*, link, get_link, set_link);

%include "../include/Port.h"
%extend Port{
    public:
        %template(set_value_d) set_value<double>;
        %template(get_value_d) get_value<double>;
        %template(get_value_i) get_value<long>;
        %template(set_value_i) set_value<long>;

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
        if _newclass: value = property(get_node, set_node)
    %}

}

%pythoncode
%{
import numpy as np
def get_value_p(self):
    if self.is_float():
        return Port.get_value_d(self)
    else:
        return Port.get_value_i(self)
def set_value_p(self, v):
    if not isinstance(v, np.ndarray):
        v = np.atleast_1d(np.array(v))
    if v.dtype.kind == 'i':
        return Port.set_value_i(self, v)
    else:
        return Port.set_value_d(self, v)
setattr(Port, 'value', property(get_value_p, set_value_p))

old_init = Port.__init__
def init(
        self,
        value = [],
        *args,
        **kwargs
        ):
    old_init(self, *args, **kwargs)
    self.value = np.atleast_1d(np.array(value))
Port.__init__ = init
%}

