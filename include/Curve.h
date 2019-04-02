//
// Created by thomas on 3/28/19.
//

#ifndef FLUOMODLIB_CURVE_H
#define FLUOMODLIB_CURVE_H

#include <vector>
#include <algorithm>
#include <string>
#include <Functions.h>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


class Curve {

    friend class Decay;

protected:
    std::string name_x;
    std::string name_y;
    std::vector<double> x;
    std::vector<double> ex;
    std::vector<double> y;
    std::vector<double> ey;

public:


    // Setter
    void set_x(double *in, int n_in);
    void set_x(std::vector<double> x_);

    void set_ex(double *in, int n_in);
    void set_x_name(std::string v);

    void set_y(double *in, int n_in);
    void set_y(std::vector<double> y_);

    void set_ey(double *in, int n_in);
    void set_y_name(std::string v);

    // Getter
    void get_x(double **out, int *n_out);
    void get_y(double **out, int *n_out);
    std::vector<double> get_x();
    std::vector<double> get_y();
    double sum();
    size_t size();

    /*!
     * Calculates the difference between the x-Axis values
     * @return
     */
    std::vector<double> get_dx();

    /*!
     * Calculates the difference between the y-Axis values
     * @return
     */
    std::vector<double> get_dy();


    // Constructor, Destructor


    Curve(
            double *x, unsigned int nx,
            double *y, unsigned int ny,
            double *ey, unsigned int ney,
            std::string name_x,
            std::string name_y
    );

    Curve(
            double *x, unsigned int nx,
            double *y, unsigned int ny,
            std::string name_x,
            std::string name_y
    );

    Curve(
            double *x, unsigned int nx,
            double *y, unsigned int ny,
            double *ey, unsigned int ney
    );

    Curve(
            double *x, unsigned int nx,
            double *y, unsigned int ny
    );

    Curve(
            double dt, unsigned int nx
    );

    Curve() = default;

    ~Curve(){};


    // Methods
    /*!
     * Shifts the x Axis by a floating point number.
     *
     * @param value
     */
    void shift(double value);

    void add(Curve v, bool valid=false);
    void add(double v);

    void sub(Curve v, bool valid=false);
    void sub(double v);

    /*!
     * This method multiplies to the y-values of the current instance of a @class Curve
     * object the y-values of another @class Curve object.
     *
     * @param v An object of the type @class Curve
     * @param valid If this parameter is false (default value) the y-values are directly multiplied
     * without check for overlap. If this parameter is true only the overlapping y-values are multiplied.
     * The overlap is judged by the values of the x-axis (TODO).
     */
    void mul(Curve v, bool valid=false);
    void mul(double v);
    void resize(size_t v);

    void save(std::string filename);
    void load(std::string filename);


};


#endif //FLUOMODLIB_CURVE_H
