//
// Created by thomas on 3/29/19.
//

#include "Decay.h"


// Methods

void Decay::append(double amplitude, double fluorescence_lifetime) {
    lifetime_spectrum.push_back(amplitude);
    lifetime_spectrum.push_back(fluorescence_lifetime);
}


void Decay::pop() {
    lifetime_spectrum.erase(lifetime_spectrum.end());
    lifetime_spectrum.erase(lifetime_spectrum.end());
}


void Decay::update() {

    double** lifetime_spectrum;
    int* n_lifetime_spectrum;
    Functions::copy_vector_to_array(Decay::lifetime_spectrum, lifetime_spectrum, n_lifetime_spectrum);

    auto *decay_out = (double*) malloc(size() * sizeof(double));
    int n_decay_out = size();

    auto *array_irf = (double*) malloc(irf.size() * sizeof(double));
    int n_array_irf = size();
    auto irf_y = irf.get_y();
    for(int i = 0; i < n_array_irf; i++){
        array_irf[i] = irf_y[i];
    }

    auto dx = get_dx();
    double dt = dx[0];

    Functions::convolve_exponentials(
            decay_out, n_decay_out,
            *lifetime_spectrum, *n_lifetime_spectrum,
            array_irf, n_array_irf,
            convolution_stop,
            dt
            );

    y.resize(0);
    for(int i=0; i < n_decay_out; i++){
        y.push_back(decay_out[i]);
    }

}


// Getter


std::vector<double> Decay::get_amplitudes() {
    int n_lifetimes = lifetime_spectrum.size() / 2;
    std::vector<double> amplitudes;

    for(int i = 0; i < n_lifetimes; i++){
        amplitudes.push_back(lifetime_spectrum[2 * i + 0]);
    }
    return amplitudes;
}


std::vector<double> Decay::get_lifetimes() {
    int n_lifetimes = lifetime_spectrum.size() / 2;
    std::vector<double> lifetimes;

    for(int i = 0; i < n_lifetimes; i++){
        lifetimes.push_back(lifetime_spectrum[2 * i + 1]);
    }
    return lifetimes;
}


// Setter

void Decay::set_fluorescence_lifetimes(double *lifetime_spectrum, int n_lifetime_spectrum) {
    Decay::lifetime_spectrum.clear();
    Decay::lifetime_spectrum.resize(n_lifetime_spectrum);

    for(int i=0; i<n_lifetime_spectrum; i++){
        Decay::lifetime_spectrum.push_back(lifetime_spectrum[i]);
    }
}


void Decay::set_instrument_response_function(Curve &irf) {
    Decay::irf = irf;
}


void Decay::set_convolution_stop(unsigned int v) {
    convolution_stop = v;
}

