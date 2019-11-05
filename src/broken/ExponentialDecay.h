//
// Created by thomas on 3/29/19.
//

#ifndef chinet_DECAY_H
#define chinet_DECAY_H


#include <vector>
#include <Curve.h>
#include <Functions.h>
#include <iostream>


class ExponentialDecay : public Curve{

private:
    std::vector<double> lifetime_spectrum;
    unsigned int convolution_start;
    unsigned int convolution_stop;
    double dt;
    Curve* irf;
    int convolution_mode;
    double period;

public:

    // Constructor
    ExponentialDecay();

    ExponentialDecay(double dt, unsigned int nx);

    ~ExponentialDecay(){
        free(irf);
    }

    // Methods
    /*!
     * Adds a fluorescence lifetime to the list of lifetimes
     * @param amplitude the amplitude of the added lifetime
     * @param lifetime
     */
    void append(double amplitude, double lifetime);

    /*!
     * Removes the lifetime from the end of the fluorescence lifetime list.
     */
    void remove_last();


    void update();

    void to_json(std::string filename);


    // Getter
    std::vector<double> get_amplitudes();
    std::vector<double> get_lifetimes();
    Curve* get_irf();
    unsigned int get_convolution_stop();

    // Setter
    void set_convolution_stop(unsigned int);
    void set_convolution_start(unsigned int);

    /*!
     * Sets the instrument response function (IRF)
     * @param irf An array containing the amplitudes of the IRF
     * @param n_irf The length of the IRF
     */
    void set_instrument_response_function(Curve *irf);
    void set_instrument_response_function(Curve *irf, unsigned int convolution_mode);

    void set_convolution_mode(unsigned int convolution_mode);

    /*!
     * Sets the values of the fluorescence lifetimes. The fluorescence lifetime array
     * @param lifetime_spectrum contains the fluorescence lifetimes and the corresponding
     * amplitudes in an interleaved array (amplitude_1, lifetime_1, amplitude_2, lifetime_2, ...)
     *
     * @param lifetime_spectrum An array containing the fluorescence lifetimes and the corresponding
     * amplitudes
     * @param n_lifetime_spectrum the number of the fluorescence lifetimes
     */
    void set_fluorescence_lifetimes(double* lifetime_spectrum, int n_lifetime_spectrum);

};


#endif //chinet_DECAY_H
