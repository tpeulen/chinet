//
// Created by thomas on 3/28/19.
//

#ifndef FLUOMODLIB_PDA_H
#define FLUOMODLIB_PDA_H

#include <math.h>


class Pda {


    /*!
     *
     * calculating p(G,R) according to Matthew
     *
     * @param SgSr  SgSr(i,j) = p(Sg=i, Sr=j)
     * @param pN p(N)
     * @param Nmax max number of photons (max N)
     * @param Bg <background green>, per time window (!)
     * @param Br <background red>, -"-
     * @param pg_theor
     */
    void sgsr_pN(
            double *SgSr,
            double *pN,
            unsigned int Nmax,
            double Bg,
            double Br,
            double pg_theor
    );


    /*!
     * calculating p(G,R), one ratio, one P(F)
     *
     * @param SgSr sgsr_pN
     * @param pF input p(F)
     * @param Nmax
     * @param Bg
     * @param Br
     * @param pg_theor
     */
    void sgsr_pF(
            double *SgSr,
            double *pF,
            unsigned int Nmax,
            double Bg,
            double Br,
            double pg_theor
    );


    /*!
     *
     * calculating p(G,R), several ratios, same P(N)
     *
     * @param SgSr see sgsr_pN
     * @param pN input: p(N)
     * @param Nmax
     * @param Bg
     * @param Br
     * @param N_pg size of pg_theor
     * @param pg_theor
     * @param a corresponding amplitudes
     */
    void sgsr_pN_manypg(
            double *SgSr,
            double *pN,
            unsigned int Nmax,
            double Bg,
            double Br,
            unsigned int N_pg,
            double *pg_theor,
            double *a);


    /*!
     *
     * calculating p(G,R), several ratios, same P(F)
     *
     * @param SgSr see sgsr_pN
     * @param pF input: p(F)
     * @param Nmax
     * @param Bg
     * @param Br
     * @param N_pg size of pg_theor
     * @param pg_theor
     * @param a corresponding amplitudes
     */
    void sgsr_pF_manypg(
            double *SgSr,
            double *pF,
            unsigned int Nmax,
            double Bg,
            double Br,
            unsigned int N_pg,
            double *pg_theor,
            double *a);

    void sgsr_manypF(double *, double *, unsigned int, double, double, unsigned int, double *, double *);


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
    void poisson_0toN(double *return_p, double lambda, unsigned int return_dim);


    /*!
     * generates Poisson distribution for a set of lambdas
     */
    void poisson_0toN_multi(double *, double *, unsigned int, unsigned int);


    /*!
     * convolves vectors p and [p2 1-p2]
     */
    void polynom2_conv(double *, double *, unsigned int, double);


};


#endif //FLUOMODLIB_PDA_H
