%module(directors="1") chinet
%feature("kwargs", 1);

%{
#include "../include/Port.h"
#include "../include/CNode.h"
%}

%include "./include/numpy.i"
%include "std_vector.i";
%init %{
    import_array();
%}

%include <std_shared_ptr.i>
%shared_ptr(Port)
%shared_ptr(Node)

%apply (double* INPLACE_ARRAY1, int DIM1) {(double *input, int n_input)}
%apply (double** ARGOUTVIEW_ARRAY1, int* DIM1) {(double **output, int *n_output)}
%apply (long* INPLACE_ARRAY1, int DIM1) {(long *input, int n_input)}
%apply (long** ARGOUTVIEW_ARRAY1, int* DIM1) {(long **output, int *n_output)}
%apply (unsigned char* INPLACE_ARRAY1, int DIM1) {(unsigned char *input, int n_input)}
%apply (unsigned char** ARGOUTVIEW_ARRAY1, int* DIM1) {(unsigned char **output, int *n_output)}

%template(vector_port_ptr) std::vector<Port*>;

%include attribute.i
%attribute(Port, bool, fixed, is_fixed, set_fixed);
%attribute(Port, bool, is_output, is_output, set_port_type);
%attribute(Port, bool, reactive, is_reactive, set_reactive);
%attribute(Port, bool, bounded, is_bounded, set_bounded);

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
    __swig_setmethods__["link"] = set_link
    __swig_getmethods__["link"] = get_link

    @property
    def value(self):
        if self.value_type == 0:
            return self.get_value_i()
        else:
            return self.get_value_d()

    @value.setter
    def value(self, v):
        if not isinstance(v, np.ndarray):
            v = np.atleast_1d(v)
        if v.dtype.kind == 'i':
            self.set_value_i(v, True)
        else:
            self.set_value_d(v, True)

    @property
    def bounds(self):
        if self.bounded:
            return self.get_bounds()
        else:
            return None, None

    @bounds.setter
    def bounds(self, v):
        self.set_bounds(np.array(v, dtype=np.float64))

    def __init__(
            self,
            value=[],
            fixed=False,
            *args, **kwargs
        ):
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
        self.fixed = fixed
    %}
}
