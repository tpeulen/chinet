//
// Created by thomas on 3/29/19.
//

#include "Decay.h"

// Constructor


Decay::Decay() :
valid_irf(false),
irf(nullptr),
convolution_stop(0),
lifetime_spectrum(0)
{
}

Decay::Decay(double dt, unsigned int nx) :
Curve(dt, nx),
valid_irf(false),
irf(nullptr),
convolution_stop(0),
lifetime_spectrum(0)
{
    irf = new Curve(dt, nx);
    irf->y[0] = 1;
    convolution_stop = irf->size();
}


// Methods

void Decay::append(double amplitude, double fluorescence_lifetime) {
    lifetime_spectrum.push_back(amplitude);
    lifetime_spectrum.push_back(fluorescence_lifetime);
}


void Decay::remove_last() {
    lifetime_spectrum.erase(lifetime_spectrum.end());
    lifetime_spectrum.erase(lifetime_spectrum.end());
}


void Decay::update() {

    if(irf != nullptr){

        // the convolution routine wants an array of the lifetime
        // spectrum
        int n_ls = lifetime_spectrum.size();
        auto *ls = (double*) malloc(n_ls * sizeof(double));
        for(int i = 0; i < n_ls; i++){
            ls[i] = lifetime_spectrum[i];
        }

        int n_decay_out = size();
        auto *decay_out = (double*) malloc(n_decay_out * sizeof(double));
        for(int i = 0; i < n_decay_out; i++) decay_out[i] = 0.0;

        int n_array_irf = size();
        auto *array_irf = (double*) malloc(irf->size() * sizeof(double));
        auto irf_y = irf->get_y();
        for(int i = 0; i < n_array_irf; i++) array_irf[i] = irf_y[i];

        auto dx = get_dx();
        double dt = dx[0];

        Functions::convolve_exponentials(
                decay_out, n_decay_out,
                ls, n_ls,
                array_irf, n_array_irf,
                convolution_stop,
                dt
        );

        for(int i=0; i < n_decay_out; i++){
            y[i] = decay_out[i];
        }

    }
}


// Getter

Curve* Decay::get_irf() {
    return irf;
}


std::vector<double> Decay::get_amplitudes() {
    int n_lifetimes = lifetime_spectrum.size() / 2;
    std::vector<double> amplitudes;

    amplitudes.resize(n_lifetimes);
    for(int i = 0; i < n_lifetimes; i++){
        amplitudes[i] = lifetime_spectrum[2 * i + 0];
    }
    return amplitudes;
}


std::vector<double> Decay::get_lifetimes() {
    int n_lifetimes = lifetime_spectrum.size() / 2;
    std::vector<double> lifetimes;

    lifetimes.resize(n_lifetimes);
    for(int i = 0; i < n_lifetimes; i++){
        lifetimes[i] = lifetime_spectrum[2 * i + 1];
    }
    return lifetimes;
}


unsigned int Decay::get_convolution_stop() {
    return convolution_stop;
}


// Setter

void Decay::set_fluorescence_lifetimes(double *lifetime_spectrum, int n_lifetime_spectrum) {
    Decay::lifetime_spectrum.clear();
    Decay::lifetime_spectrum.resize(n_lifetime_spectrum);

    for(int i=0; i<n_lifetime_spectrum; i++){
        Decay::lifetime_spectrum.push_back(lifetime_spectrum[i]);
    }
}


void Decay::set_instrument_response_function(Curve *v) {
    irf = new Curve();
    irf->set_y(v->get_y());
    irf->set_x(v->get_x());
}


void Decay::set_convolution_stop(unsigned int v) {
    convolution_stop = v;
}

