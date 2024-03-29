#include "include/_old/Curve.h"


// Constructor

Curve::Curve(
        double *x, unsigned int nx,
        double *y, unsigned int ny,
        double *ey, unsigned int ney,
        std::string name_x,
        std::string name_y
){
    set_x(x, nx);
    set_y(y, ny);

    // if no errors are provided use 1.0 as an error
    if(ney == 0){
        auto r = (double*) malloc(sizeof(double) *  ny);
        for(int i=0; i < ny; i++) r[i] = 1.0;
        set_ey(r, ny);
    } else{
        set_ey(ey, ney);
    }

    set_x_name(name_x);
    set_y_name(name_y);
}


Curve::Curve(double *x, unsigned int nx, double *y, unsigned int ny) :
Curve(x, nx, y, ny, nullptr, 0, std::string(""), std::string(""))
{}


Curve::Curve(double *x, unsigned int nx, double *y, unsigned int ny, double *ey, unsigned int ney) :
Curve(x, nx, y, ny, ey, ney, std::string(""), std::string(""))
{}


Curve::Curve(double *x, unsigned int nx, double *y, unsigned int ny, std::string name_x, std::string name_y) :
Curve(x, nx, y, ny, nullptr, 0, name_x, name_y)
{}


Curve::Curve(double dt, unsigned int nx) {

    std::vector<double> x_(nx);
    for(int i = 0; i < nx; i++){
        x_[i] = i * dt;
    }
    set_x(x_);

    std::vector<double> y_(nx);
    for(int i = 0; i < nx; i++){
        x_[i] = 0.0;
    }
    set_y(y_);

}


// Methods
Curve Curve::operator+(Curve &v) {
    if(!valid_op){
        for(unsigned int i=0; i < y.size(); i++){
            y[i] += v.y[i];
        }
        return *this;
    } else{
        // TODO only add the region where the x-values overlap
        return *this;
    }
}


Curve Curve::operator+(double v) {
    for(auto &yi : y){
        yi += v;
    }
    return *this;
}


Curve Curve::operator-(Curve &v) {
    if(!valid_op){
        for(unsigned int i=0; i < y.size(); i++){
            y[i] -= v.y[i];
        }
        return *this;
    } else{
        return *this;
        // TODO (only overlapping region)
    }
}


Curve Curve::operator-(double v) {
    for(auto &yi : y){
        yi -= v;
    }
    return *this;
}


Curve Curve::operator/(Curve &v) {
    if(!valid_op){
        for(unsigned int i=0; i < y.size(); i++){
            y[i] /= v.y[i];
        }
        return *this;
    } else{
        return *this;
        // TODO (only overlapping region)
    }
}


Curve Curve::operator/(double v) {
    for(auto &yi : y){
        yi /= v;
    }
    return *this;
}


Curve Curve::operator*(Curve &v) {
    if(!valid_op){
        // simply multiply the y-values
        for(unsigned int i=0; i < y.size(); i++){
            y[i] *= v.y[i];
        }
        return *this;
    } else{
        // only multiply the y-values
        // the the same x-values (TODO)
        return *this;
    }
}


Curve Curve::operator*(double v) {
    for(auto &yi : y){
        yi *= v;
    }
    return *this;
}


void Curve::shift(double value) {
    Functions::shift(value, y);
    Functions::shift(value, ey);
}


double Curve::sum() {
    double r = 0;
    for(auto vi : y){
        r += vi;
    }
    return r;
}


void Curve::resize(size_t v) {
    y.resize(v);
    x.resize(v);
}


void Curve::to_json(std::string filename) {
    /*
    json jsonfile;

    jsonfile["type"] = "Curve";

    jsonfile["x"]["values"] = x;
    jsonfile["x"]["name"] = name_x;

    jsonfile["y"]["values"] = y;
    jsonfile["y"]["errors"] = ey;
    jsonfile["y"]["name"] = name_y;

    std::ofstream file(filename);
    file << jsonfile;
     */
}


void Curve::from_json(std::string filename) {
    /*

    x.clear();
    y.clear();
    ey.clear();

    std::ifstream ifs(filename);
    json j = json::parse(ifs);

    for(const auto &xi : j["x"]["values"]){
        x.push_back(xi);
    }

    for(const auto &yi : j["y"]["values"]){
        y.push_back(yi);
    }

    for(const auto &yi : j["y"]["errors"]){
        ey.push_back(yi);
    }

    name_x = j["x"]["name"];
    name_y = j["y"]["name"];
     */
}



// Setter

void Curve::set_x_name(std::string v) {
    name_x = v;
}


void Curve::set_y_name(std::string v) {
    name_y = v;
}


void Curve::set_x(double *in, int n_in) {
    Functions::copy_array_to_vector(in, n_in, x);
}


void Curve::set_ey(double *in, int n_in) {
    Functions::copy_array_to_vector(in, n_in, ey);
}


void Curve::set_ex(double *in, int n_in) {
    Functions::copy_array_to_vector(in, n_in, ex);
}


void Curve::set_y(double *in, int n_in) {
    Functions::copy_array_to_vector(in, n_in, y);

    // if the error vector is smaller than the data vector.
    // resize the error vector and fill it with ones.
    if(ey.size() < y.size()){
        int end = ey.size();
        ey.resize(y.size());
        for(int i = end - 1; i < ey.size(); i++){
            ey[i] = 1.0;
        }
    }
}


void Curve::set_x(std::vector<double> x_) {
    x = x_;
}


void Curve::set_y(std::vector<double> y_) {
    y = y_;
}


// Getter

void Curve::get_x(double **out, int *n_out) {
    Functions::copy_vector_to_array(x, out, n_out);
}


void Curve::get_y(double **out, int *n_out) {
    Functions::copy_vector_to_array(y, out, n_out);
}

std::vector<double> Curve::get_x() {
    return x;
}


std::vector<double> Curve::get_y() {
    return y;
}


size_t Curve::size() {
    return x.size();
}


std::vector<double> Curve::get_dx() {
    return Functions::diff(x);
}


std::vector<double> Curve::get_dy() {
    return Functions::diff(y);
}

