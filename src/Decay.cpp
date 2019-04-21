//
// Created by thomas on 3/29/19.
//

#include "Decay.h"

// Constructor


Decay::Decay() :
irf(nullptr),
convolution_start(0),
convolution_stop(0),
convolution_mode(0),
period(0),
lifetime_spectrum(0)
{
}


Decay::Decay(double dt, unsigned int nx) :
Curve(dt, nx),
irf(nullptr),
convolution_start(0),
convolution_stop(0),
period(0),
convolution_mode(0),
lifetime_spectrum(0)
{
    irf = new Curve(dt, nx);
    irf->y[0] = 1;
    convolution_stop = irf->size();
    Decay::dt = get_dx()[0];
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

    switch (convolution_mode){
        case 1:
            Functions::convolve_sum_of_exponentials_periodic(
                    y.data(), y.size(),
                    lifetime_spectrum.data(), lifetime_spectrum.size(),
                    irf->y.data(), irf->y.size(),
                    convolution_start,
                    convolution_stop,
                    dt,
                    period
            );
            break;
        default:
            Functions::convolve_sum_of_exponentials(
                    y.data(), y.size(),
                    lifetime_spectrum.data(), lifetime_spectrum.size(),
                    irf->y.data(), irf->y.size(),
                    convolution_stop,
                    dt
            );
            break;
    }

}


void Decay::to_json(std::string filename) {
    /*
    json jsonfile;

    jsonfile["type"] = "Decay";

    jsonfile["x"]["values"] = x;
    jsonfile["x"]["name"] = name_x;

    jsonfile["y"]["values"] = y;
    jsonfile["y"]["errors"] = ey;
    jsonfile["y"]["name"] = name_y;

    std::ofstream file(filename);
    file << jsonfile;
     */
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
    Functions::copy_array_to_vector(lifetime_spectrum, n_lifetime_spectrum, Decay::lifetime_spectrum);
}


void Decay::set_instrument_response_function(Curve *v, unsigned int convolution_mode) {
    irf = v;
    Decay::convolution_mode = convolution_mode;
}


void Decay::set_instrument_response_function(Curve *v) {
    set_instrument_response_function(v, convolution_mode);
}


void Decay::set_convolution_mode(unsigned int convolution_mode) {
    Decay::convolution_mode = convolution_mode;
}


void Decay::set_convolution_stop(unsigned int v) {
    convolution_stop = v;
}


void Decay::set_convolution_start(unsigned int v) {
    convolution_start = v;
}

