//
// Created by thomas on 3/28/19.
//

#ifndef FLUOMODLIB_CURVE_H
#define FLUOMODLIB_CURVE_H

#include <vector>
#include <algorithm>
#include <string>
#include <Functions.h>


class Curve {

private:
    std::string name_x;
    std::string name_y;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> ey;

public:


    // Setter
    void set_x(double *in, int n_in);
    void set_x_name(std::string v);
    void set_y(double *in, int n_in);
    void set_y_name(std::string v);
    void set_ey(double *in, int n_in);


    // Getter
    void get_x(double **out, int *n_out);
    void get_y(double **out, int *n_out);
    std::vector<double> get_x();
    std::vector<double> get_y();


    // Constructor, Destructor

    Curve() = default;

    Curve(
            double *x, unsigned int nx,
            double *y, unsigned int ny,
            double *ey, unsigned int ney,
            std::string name_x,
            std::string name_y
    );

    Curve(
            double *x, unsigned int nx,
            double *y, unsigned int ny
          );

    Curve(
            double *x, unsigned int nx,
            double *y, unsigned int ny,
            double *ey, unsigned int ney
            );

    Curve(
            double *x, unsigned int nx,
            double *y, unsigned int ny,
            std::string name_x,
            std::string name_y
    );


    ~Curve(){};


    // Methods
    /*!
     * Shifts the x Axis by a floating point number.
     *
     * @param value
     */
    void shift(double value);

    void add(Curve v);
    void add(double v);

    void sub(Curve v);
    void sub(double v);

    void mul(Curve v);
    void mul(double v);

};


#endif //FLUOMODLIB_CURVE_H
