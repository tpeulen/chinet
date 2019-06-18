%module(directors="1") chinet

%{
#include "../include/Port.h"
#include "../include/PortLinks.h"
%}

%include "numpy.i"
%init %{
    import_array();
%}

%shared_ptr(BasePort)
%shared_ptr(ValuePort)
%shared_ptr(Port)

%template(vector_port_ptr) std::vector<Port*>;

%apply (double* IN_ARRAY1, int DIM1) {(double *in, int n_in)}
%apply (double** ARGOUTVIEW_ARRAY1, int* DIM1) {(double **out, int *n_out)}

%ignore get_value(); // use memory - void get_value(double **out, int *nbr_out)
%ignore set_value(std::vector<double> &values); // use memory - void get_value(double **out, int *nbr_out)

%include attribute.i
%attribute(Port, bool, fixed, is_fixed, set_fixed);
%attribute(Port, bool, is_output, is_output, set_port_type);
%attribute(Port, bool, reactive, is_reactive, set_reactive);
%attribute(Port, bool, bounded, is_bounded, set_bounded);
%attributestring(Port, std::string, name, get_name, set_name);
//%attribute(Port, std::vector<double>, bounds, get_bounds, set_bounds);

%include "../include/Port.h"
%include "../include/PortLinks.h"

%extend Port {

        std::string __repr__(){
            std::ostringstream os;
            os << "Port: ";
            os << $self->get_json();
            os << "\n";
            return os.str();
        }

        %pythoncode
        %{
            __swig_getmethods__["value"] = get_value
            __swig_setmethods__["value"] = set_value
            if _newclass: value = property(get_value, set_value)
        %}

}


