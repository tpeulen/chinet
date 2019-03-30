//
// Created by thomas on 3/29/19.
//

#ifndef FLUOMODLIB_FUNCTIONS_H
#define FLUOMODLIB_FUNCTIONS_H

#include <vector>
#include <algorithm>


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
    void copy_vector_to_array(std::vector<double> &v, double **out, int *nout);


};


#endif //FLUOMODLIB_FUNCTIONS_H
