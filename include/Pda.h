//
// Created by thomas on 3/28/19.
//

#ifndef FLUOMODLIB_PDA_H
#define FLUOMODLIB_PDA_H

#include <math.h>


class Pda {

    void sgsr_pN(double*, double*, unsigned int, double, double, double);

    void sgsr_pF(double*, double*, unsigned int, double, double, double);

    void sgsr_pN_manypg (double*, double*, unsigned int, double, double, unsigned int, double*, double*);

    void sgsr_pF_manypg (double*, double*, unsigned int, double, double, unsigned int, double*, double*);

    void sgsr_manypF (double*, double*, unsigned int, double, double, unsigned int, double*, double*);


    /*!
     *
     * @param SgSr
     * @param FgFr
     * @param Nmax
     * @param Bg
     * @param Br
     */
    void conv_pF(double *SgSr, double *FgFr, unsigned int Nmax, double Bg, double Br);


    /*!
     *
     * @param SgSr
     * @param FgFr
     * @param Nmax
     * @param Bg
     * @param Br
     * @param pN
     */
    void conv_pN(double *SgSr, double *FgFr, unsigned int Nmax, double Bg, double Br, double *pN);


    /*!
    * generates Poisson distribution witn average= lambda, for 0..N
    *
    * @param return_p
    * @param lambda
    * @param return_dim
    */
    void poisson_0toN (double* return_p, double lambda, unsigned int return_dim);


    /*!
     * generates Poisson distribution for a set of lambdas
     */
    void poisson_0toN_multi (double*, double*, unsigned int, unsigned int);


    /*!
     * convolves vectors p and [p2 1-p2]
     */
    void polynom2_conv (double*, double*, unsigned int, double);


};


#endif //FLUOMODLIB_PDA_H
