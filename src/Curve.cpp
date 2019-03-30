//
// Created by thomas on 3/28/19.
//

#include "../include/Curve.h"

// Constructor

Curve::Curve(
        double *x, unsigned int nx,
        double *y, unsigned int ny,
        double *ey, unsigned int ney,
        std::string name_x,
        std::string name_y
){
    set_x(x, nx);
    set_y(y, ny);

    // if no errors are provided use 1.0 as an error
    if(ney == 0){
        auto r = (double*) malloc(sizeof(double) *  ny);
        for(int i=0; i < ny; i++) r[i] = 1.0;
        set_ey(r, ny);
    } else{
        set_ey(ey, ney);
    }

    set_x_name(name_x);
    set_y_name(name_y);
}


Curve::Curve(double *x, unsigned int nx, double *y, unsigned int ny) :
Curve(x, nx, y, ny, nullptr, 0, std::string(""), std::string(""))
{}

Curve::Curve(double *x, unsigned int nx, double *y, unsigned int ny, double *ey, unsigned int ney) :
Curve(x, nx, y, ny, ey, ney, std::string(""), std::string(""))
{}


Curve::Curve(double *x, unsigned int nx, double *y, unsigned int ny, std::string name_x, std::string name_y) :
Curve(x, nx, y, ny, nullptr, 0, name_x, name_y)
{}


// Methods

void Curve::add(Curve v) {
    for(unsigned int i=0; i < y.size(); i++){http://www.cplusplus.com/reference/vector/vector/vector/
        y[i] += v.y[i];
    }
}


void Curve::add(double v) {
    for(auto &yi : y){
        yi += v;
    }
}


void Curve::sub(Curve v) {
    for(unsigned int i=0; i < y.size(); i++){
        y[i] += v.y[i];
    }
}


void Curve::sub(double v) {
    for(auto &yi : y){
        yi += v;
    }
}


void Curve::mul(Curve v) {
    for(unsigned int i=0; i < y.size(); i++){
        y[i] *= v.y[i];
    }
}


void Curve::mul(double v) {
    for(auto &yi : y){
        yi *= v;
    }
}


void Curve::shift(double value) {
    Functions::shift(value, y);
    Functions::shift(value, ey);
}


// Setter

void Curve::set_x_name(std::string v) {
    name_x = v;
}


void Curve::set_y_name(std::string v) {
    name_y = v;
}


void Curve::set_x(double *in, int n_in) {
    Curve::x.resize(n_in);
    for(int i=0; i<n_in; i++){
        Curve::x[i] = in[i];
    }
}


void Curve::set_ey(double *in, int n_in) {
    Curve::ey.resize(n_in);
    for(int i=0; i<n_in; i++){
        Curve::ey[i] = x[i];
    }
}


void Curve::set_y(double *in, int n_in) {
    y.resize(n_in);
    for(int i=0; i<n_in; i++){
        y[i] = in[i];
    }
    // if the error vector is smaller than the data vector.
    // resize the error vector and fill it with ones.
    if(ey.size() < y.size()){
        int end = ey.size();
        ey.resize(y.size());
        for(int i = end - 1; i < ey.size(); i++){
            ey[i] = 1.0;
        }
    }
}


// Getter

void Curve::get_x(double **out, int *n_out) {
    Functions::copy_vector_to_array(x, out, n_out);
}


void Curve::get_y(double **out, int *n_out) {
    Functions::copy_vector_to_array(y, out, n_out);
}


std::vector<double> Curve::get_x() {
    return x;
}


std::vector<double> Curve::get_y() {
    return y;
}


