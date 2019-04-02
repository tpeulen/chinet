//
// Created by thomas on 3/29/19.
//

#include "Decay.h"

// Constructor


Decay::Decay() :
valid_irf(false),
irf(nullptr),
convolution_stop(0),
lifetime_spectrum(0),
polarization("vm")
{
}


Decay::Decay(double dt, unsigned int nx, std::string polarization) :
Curve(dt, nx),
valid_irf(false),
irf(nullptr),
convolution_stop(0),
lifetime_spectrum(0)
{
    irf = new Curve(dt, nx);
    irf->y[0] = 1;
    convolution_stop = irf->size();
    Decay::dt = get_dx()[0];
    Decay::polarization = polarization;
}

Decay::Decay(double dt, unsigned int nx) :
        Decay(dt, nx, "vm")
{
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

    Functions::convolve_exponentials(
            y.data(), y.size(),
            lifetime_spectrum.data(), lifetime_spectrum.size(),
            irf->y.data(), irf->y.size(),
            convolution_stop,
            dt
    );

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
    irf = v;
}


void Decay::set_convolution_stop(unsigned int v) {
    convolution_stop = v;
}

