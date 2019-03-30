//
// Created by thomas on 3/29/19.
//

#include "Functions.h"


void Functions::shift(double value, std::vector<double> &x) {
    double ts = -value;
    int tsi = (int) ts;
    double tsf = ts - tsi;

    // shift the values contained in the temporary the vector the integer tsi
    Functions::roll(tsi, x);

    // make a copy and roll it by tsi + 1
    std::vector<double> x_(x);
    Functions::roll(tsi + 1, x_);

    // make the
    // scale the values contained in the temporary y vector by tsf
    for(unsigned int i=0; i<x.size(); i++){
        x[i] = x[i] * tsf + x_[i] * (1. - tsf);
    }

}


void Functions::roll(int value, std::vector<double> &y){
    if(value > 0){
        std::rotate(y.begin(), y.begin() + value, y.end());
    } else{
        value = value * (-1);
        std::rotate(y.rbegin(), y.rbegin() + value, y.rend());
    }
}

void Functions::copy_vector_to_array(std::vector<double> &v, double **out, int *nout){
    auto r = (double*) malloc(v.size() * sizeof(double));
    for(unsigned int i = 0; i < v.size(); i++){
        r[i] = v[i];
    }
    *out = r;
    *nout = v.size();
}