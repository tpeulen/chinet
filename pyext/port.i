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

%apply (double* IN_ARRAY1, int DIM1) {(double *in, int nbr_in)}
%apply (double** ARGOUTVIEW_ARRAY1, int* DIM1) {(double **out, int *nbr_out)}
%ignore get_value(); // use memory - void get_value(double **out, int *nbr_out)
%ignore set_value(std::vector<double> &values); // use memory - void get_value(double **out, int *nbr_out)

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

        /*
        std::shared_ptr<Port> __getitem__(std::string key)
        {
                return (*self)[key];
        };
         */

}

