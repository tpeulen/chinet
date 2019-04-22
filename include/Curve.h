//
// Created by thomas on 3/28/19.
//

#ifndef chinet_CURVE_H
#define chinet_CURVE_H

#include <vector>
#include <algorithm>
#include <string>
#include <Functions.h>
#include <fstream>
#include <bson.h>
#include <Port.h>


class Curve : Port{

    friend class Decay;

private:
    // if this is true only overlapping regions of
    // curves are operated on addition, etc.
    bool valid_op = false;

    // if this is true operations are executed in place, i.e., no new
    // objects are created.
    bool inplace_op = true;

    // BSON
    bson_t document;

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

    /*!
     * This method multiplies to the y-values of the current instance of a @class Curve
     * object the y-values of another @class Curve object.
     *
     * @param v An object of the type @class Curve
     * @param valid If this parameter is false (default value) the y-values are directly multiplied
     * without check for overlap. If this parameter is true only the overlapping y-values are multiplied.
     * The overlap is judged by the values of the x-axis (TODO).
     */

    Curve operator+(Curve &v);
    Curve operator+(double v);

    Curve operator-(Curve &v);
    Curve operator-(double v);

    Curve operator*(Curve &v);
    Curve operator*(double v);

    Curve operator/(Curve &v);
    Curve operator/(double v);

    void resize(size_t v);

    virtual void to_json(std::string filename);
    virtual void from_json(std::string filename);


};


#endif //chinet_CURVE_H
