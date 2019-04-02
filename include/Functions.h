//
// Created by thomas on 3/29/19.
//

#ifndef FLUOMODLIB_FUNCTIONS_H
#define FLUOMODLIB_FUNCTIONS_H
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#include <vector>
#include <algorithm>
#include <math.h>
#include <iostream>


namespace Functions {


    /*!
     * Shifts a vector by a floating number.
     *
     * This function shifts the y-axis and consider non-integer values by
     * determining the integer part of the shift and the
     * floating part of the shift value, e.g., for the shift 4.66
     * the integer part is 4 and the floating part is 0.66.
     * Next the array is shifted by the integer part and a copy
     * of the array is shifted by the integer part + 1. Finally,
     * the weighted sum of the both arrays is calculated.
     * @param value
     * @param x
     */
    void shift(double value, std::vector<double> &x);


    /*!
     * Rolls a vector by an integer
     *
     * @param value
     * @param y
     */
    void roll(int value, std::vector<double> &y);


    /*!
     * Allocates memory on a pointer to an array and copies the content
     * of a vector to the newly allocated memory.
     *
     * This function is mainly used for numpy array SWIGs
     *
     * @param v
     * @param out
     * @param nout
     */
    void copy_vector_to_array(
            std::vector<double> &v,
            double **out, int *nout);

    /*!
     * Copy an array and copies the content
     * of a vector to already allocated memory.
     *
     * This function is mainly used for numpy array SWIGs
     *
     * @param v
     * @param out
     * @param nout
     */
    void copy_vector_to_array(
            std::vector<double> &v,
            double *out, int nout);

    /*!
     *
     * @param in
     * @param nin
     * @param v
     */
    void copy_array_to_vector(double *in, int nin, std::vector<double> &v);


    /*!
     * This function copies two vectors of equal size to an interleaved array
     *
     * @param v1
     * @param v2
     * @param out
     * @param nout
     */
    void copy_two_vectors_to_interleaved_array(
            std::vector<double> &v1,
            std::vector<double> &v2,
            double**out, int *nout
    );


    /*!
     * This function convolves a sum of exponential decays with an instrument response function (IRF)
     *
     * The sum of exponential decays is passed by the lifetime array @param lifetime_spectrum that contains the amplitudes
     * and the corresponding fluorescence lifetimes in an interleaved array (amplitude_1, lifetime_1, amplitude_2,
     * lifetime_2, ...). The instrument response function is passed by the array @param irf. The IRF and the and
     * the sum of exponential decays are convolved up to the index @param convolution_stop. In this convolution,
     * the time axis is uniform an linear with a time interval specified by the parameter @param dt.
     *
     * @param out The array to which the convoluted decay is written to
     * @param n_out The number of elements in the output array
     * @param lifetime_spectrum The array containing the fluorescence lifetimes and the amplitudes
     * @param n_lifetime_spectrum The number of fluorescence lifetimes
     * @param irf The array containing the instrument response function
     * @param n_irf The number of elements of the instrument response function
     * @param convolution_stop The stop of the convolution
     * @param dt The time resolution
     *
     */
    void convolve_exponentials(
            double* out, int n_out,
            const double* lifetime_spectrum, int n_lifetime_spectrum,
            const double* irf, int n_irf,
            int convolution_stop,
            double dt);

    /*!
     * This function convolves a sum of exponential decays with an instrument response function (IRF)
     *
     * The sum of exponential decays is passed by the lifetime array @param lifetime that contains the amplitudes
     * and the corresponding fluorescence lifetimes in an interleaved array (amplitude_1, lifetime_1, amplitude_2,
     * lifetime_2, ...). The instrument response function is passed by the array @param irf. The IRF and the and
     * the sum of exponential decays are convolved up to the index @param convolution_stop. In this convolution,
     * the time axis is uniform an linear with a time interval specified by the parameter @param dt.
     *
     * @param out The array to which the convoluted decay is written to
     * @param n_out The number of elements in the output array
     * @param lifetime The array containing the fluorescence lifetimes and the amplitudes
     * @param n_lifetimes The number of fluorescence lifetimes
     * @param irf The array containing the instrument response function
     * @param n_irf The number of elements of the instrument response function
     * @param start The start index of the convolution
     * @param stop The stop index of the convolution
     * @param dt The time resolution
     * @param period The time between repeated excitation intervals
     *
     */
    void convolve_exponentials_periodic(
        double* out, int n_out,
        const double* lifetime, int n_lifetimes,
        const double* irf, int n_irf,
        int start,
        int stop,
        double dt,
        double period
        );

}


#endif //FLUOMODLIB_FUNCTIONS_H
