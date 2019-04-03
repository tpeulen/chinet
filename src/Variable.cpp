//
// Created by thomas on 4/2/19.
//

#include <include/Variable.h>
#include "Variable.h"

// Constructor


Variable::Variable()
{}


Variable::Variable(double v, std::string n)
:
value(v),
name(n)
{}


Variable::Variable(double v) :
Variable(v, std::string(" "))
{}


// Operators

Variable Variable::operator*(Variable &v){
    Variable r;
    r.linked_variables.push_back(this);
    r.linked_variables.push_back(&v);
    r.name = make_name("mul", r.linked_variables);
    r.eval = mul;
    return r;
}

std::string Variable::make_name(const std::string function_name, std::vector<Variable *> v) {
    std::string r;
    r.append(function_name);
    r.append("(");
    for(auto vi : v){
        r.append(vi->name);
        r.append(",");
    }
    r.append(")");
    return r;
}


double mul(std::vector<Variable*> &v){
    double r = 1.0;
    for(auto vi : v){
        if(vi != nullptr){
            r *= vi->get_value();
        }
    }
    return r;
}


// Getter

std::vector<Variable*> Variable::get_linked_variables(){
    return linked_variables;
}


double Variable::get_value(){
    if(auto_update){
        update();
    }
    return value;
}


std::string Variable::get_name(){
    return name;
}





