//
// Created by thomas on 3/29/19.
//

#ifndef FLUOMODLIB_DECAY_H
#define FLUOMODLIB_DECAY_H


#include <vector>
#include <Curve.h>
#include <Functions.h>


class Decay : public Curve{

private:
    std::vector<double> lifetime_spectrum;
    Curve irf;
    unsigned int convolution_stop;

public:

    // Methods

    /*!
     * Adds a fluorescence lifetime to the list of lifetimes
     * @param amplitude the amplitude of the added fluorescence lifetime
     * @param fluorescence_lifetime the fluorescence lifetime
     */
    void append(double amplitude, double fluorescence_lifetime);

    /*!
     * Removes the fluorescence lifetime from the end of the fluorescence lifetime list.
     */
    void pop();


    void update();


    // Getter

    std::vector<double> get_amplitudes();
    std::vector<double> get_lifetimes();

    // Setter


    void set_convolution_stop(unsigned int);

    /*!
     * Sets the instrument response function (IRF)
     * @param irf An array containing the amplitudes of the IRF
     * @param n_irf The length of the IRF
     */
    void set_instrument_response_function(Curve &irf);

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


#endif //FLUOMODLIB_DECAY_H
