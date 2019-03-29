//
// Created by thomas on 3/28/19.
//

#ifndef FLUOMODLIB_FLUORESCENCEDECAY_H
#define FLUOMODLIB_FLUORESCENCEDECAY_H

#include <std>
#include <vector>



class FluorescenceDecay {

private:
    std::vector<double <std::vector<double>> fluorescence_lifetimes;
    std::vector<double> irf;


public:
    /*!
     * Adds a fluorescence lifetime to the list of lifetimes
     * @param amplitude the amplitude of the added fluorescence lifetime
     * @param fluorescence_lifetime the fluorescence lifetime
     */
    void append(double amplitude, double fluorescence_lifetime);

    /*!
     * Removes the fluorescence lifetime from the specified position
     * @param position the position of the fluorescence lifetime that is removed.
     */
    void pop(size position);

    /*!
     * Removes the fluorescence lifetime from the end of the fluorescence lifetime list.
     */
    void pop();

    /*!
     * Sets the instrument response function (IRF)
     * @param irf An array containing the amplitudes of the IRF
     * @param n_irf The length of the IRF
     */
    void set_instrument_response_function(double* irf, size_t n_irf);

    /*!
     * Sets the values of the fluorescence lifetimes.
     * @param fluorescence_lifetimes
     */
    void set_fluorescence_lifetimes(double* fluorescence_lifetimes);

};


#endif //FLUOMODLIB_FLUORESCENCEDECAY_H
