#ifndef FLUOMODLIB_VARIABLE_H
#define FLUOMODLIB_VARIABLE_H

#include <string>
#include <vector>



class Variable {

private:

    /// the value of the variable
    double value;

    /// a list of variables the are used to compute the value of the variable
    std::vector<Variable*> linked_variables;

    /// the pointer to the function that evaluates the linked input variables to yield the output
    double (*eval)(std::vector<Variable*> &v);

    /// the name of the variable (only used of representation)
    std::string name;

    /// True if the inputs and the value are valid
    bool valid;

    /// if true the variable is updated on each call of get_value
    bool auto_update = true;


public:

    // Constructor
    Variable(double v, std::string n);
    explicit Variable(double v);
    Variable();


    // Operators
    Variable operator*(Variable &v);

    /*
    Variable operator+(Variable &v);
    Variable operator-(Variable &v);
    Variable operator/(Variable &v);
    */

    void update(){
        //return value;
        if(eval){
            value = eval(linked_variables);
        }
    }

    void link(
            std::vector<Variable*> &v,
            double(*pfn)(std::vector<Variable*>& v)
            ){
        eval = pfn;
        linked_variables = v;
    }

    // Getter
    double get_value();
    std::string get_name();
    std::vector<Variable*> get_linked_variables();

    // Methods
    bool is_linked(){
        return (!linked_variables.empty());
    }

    std::string make_name(std::string function_name, std::vector<Variable*> v);

    bool is_valid(){
        if(linked_variables.empty()){
            return true;
        } else{
            for(auto v : linked_variables){
                if(!v->is_valid()){
                    return false;
                }
            }
            return true;
        }
    }
};

double mul(std::vector<Variable*> &v);

typedef double (*eval_t)(std::vector<Variable*> &v);

eval_t get_mul(){
    return mul;
}


#include <src/Variable.cpp>

#endif //FLUOMODLIB_VARIABLE_H
