//
// Created by thomas on 3/29/19.
//

#include "ExponentialDecay.h"

// Constructor


ExponentialDecay::ExponentialDecay() :
irf(nullptr),
convolution_start(0),
convolution_stop(0),
convolution_mode(0),
period(0),
lifetime_spectrum(0)
{
}


ExponentialDecay::ExponentialDecay(double dt, unsigned int nx) :
Curve(dt, nx),
irf(nullptr),
convolution_start(0),
convolution_stop(0),
period(0),
convolution_mode(0),
lifetime_spectrum(0)
{
    irf = new Curve(dt, nx);
    irf->set_y(
            std::vector<double>{1}
            );
    convolution_stop = irf->size();
    ExponentialDecay::dt = get_dx()[0];
}


// Methods

void ExponentialDecay::append(double amplitude, double lifetime) {
    lifetime_spectrum.push_back(amplitude);
    lifetime_spectrum.push_back(lifetime);
}


void ExponentialDecay::remove_last() {
    lifetime_spectrum.erase(lifetime_spectrum.end());
    lifetime_spectrum.erase(lifetime_spectrum.end());
}


void ExponentialDecay::update() {

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


void ExponentialDecay::to_json(std::string filename) {
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
Curve* ExponentialDecay::get_irf() {
    return irf;
}


std::vector<double> ExponentialDecay::get_amplitudes() {
    int n_lifetimes = lifetime_spectrum.size() / 2;
    std::vector<double> amplitudes;

    amplitudes.resize(n_lifetimes);
    for(int i = 0; i < n_lifetimes; i++){
        amplitudes[i] = lifetime_spectrum[2 * i + 0];
    }
    return amplitudes;
}


std::vector<double> ExponentialDecay::get_lifetimes() {
    int n_lifetimes = lifetime_spectrum.size() / 2;
    std::vector<double> lifetimes;

    lifetimes.resize(n_lifetimes);
    for(int i = 0; i < n_lifetimes; i++){
        lifetimes[i] = lifetime_spectrum[2 * i + 1];
    }
    return lifetimes;
}


unsigned int ExponentialDecay::get_convolution_stop() {
    return convolution_stop;
}


// Setter
void ExponentialDecay::set_fluorescence_lifetimes(double *lifetime_spectrum, int n_lifetime_spectrum) {
    Functions::copy_array_to_vector(lifetime_spectrum, n_lifetime_spectrum, ExponentialDecay::lifetime_spectrum);
}


void ExponentialDecay::set_instrument_response_function(Curve *v, unsigned int convolution_mode) {
    irf = v;
    ExponentialDecay::convolution_mode = convolution_mode;
}


void ExponentialDecay::set_instrument_response_function(Curve *v) {
    set_instrument_response_function(v, convolution_mode);
}


void ExponentialDecay::set_convolution_mode(unsigned int convolution_mode) {
    ExponentialDecay::convolution_mode = convolution_mode;
}


void ExponentialDecay::set_convolution_stop(unsigned int v) {
    convolution_stop = v;
}


void ExponentialDecay::set_convolution_start(unsigned int v) {
    convolution_start = v;
}

