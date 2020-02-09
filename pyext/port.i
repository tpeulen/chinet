%module(directors="1") chinet

%{
#include "../include/Port.h"
%}

%include "./include/numpy.i"
%init %{
    import_array();
%}

%apply (double* IN_ARRAY1, int DIM1) {(double *in, int n_in)}
%apply (double** ARGOUTVIEW_ARRAY1, int* DIM1) {(double **out, int *n_out)}

%shared_ptr(Port)
%template(vector_port_ptr) std::vector<Port*>;
%include attribute.i
%attribute(Port, bool, fixed, is_fixed, set_fixed);
%attribute(Port, bool, is_output, is_output, set_port_type);
%attribute(Port, bool, reactive, is_reactive, set_reactive);
%attribute(Port, bool, bounded, is_bounded, set_bounded);
%attributestring(Port, std::string, name, get_name, set_name);
// %attribute(Port, std::vector<double>, bounds, get_bounds, set_bounds);

%include "../include/Port.h"
%extend Port {

    public:

        %template(set_value_double) set_value<double>;
        %template(get_value_double) get_value<double>;
        %template(set_value_vector) set_value_vector<double>;
        %template(get_value_vector) get_value_vector<double>;

    std::string __repr__(){
        std::ostringstream os;
        os << "Port: ";
        os << $self->get_json();
        os << std::endl;
        return os.str();
    }

    %pythoncode
    %{
            __swig_getmethods__["value"] = get_value_double
            __swig_setmethods__["value"] = set_value_double
            if _newclass: value = property(get_value_double, set_value_double)
    %}

}

