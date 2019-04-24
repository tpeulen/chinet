//
// Created by thomas on 4/22/19.
//

#ifndef CHINET_VALUE_H
#define CHINET_VALUE_H

#include <string>
#include <bson.h>
#include <Link.h>
#include <Port.h>

class Link;
class Port;


class Value {
    friend Link;
    friend Port;

private:
    bson_oid_t oid;
    bool fixed;
    std::vector<double> content;
    Value *precursor;
    Port *port;

public:
    bson_t* to_bson();
    std::string to_json();
    Value();
    Value(std::vector<double> v, bool fixed);
    Value(std::vector<double> v, bool fixed, Value *precursor);
    ~Value();

    std::vector<double> get_value();
    uint64_t get_time();
    bool push_back(double v);
};


#endif //CHINET_VALUE_H
