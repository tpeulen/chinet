//
// Created by thomas on 4/22/19.
//

#ifndef CHINET_VALUE_H
#define CHINET_VALUE_H

#include <string>
#include <bson.h>

#include <MongoObject.h>
#include <Link.h>
//#include <Port.h>


class MongoObject;


template<typename T>
class Value{

private:
    bool fixed;
    std::vector<T> content;

public:
    /*
    Port *port;
    bson_t* to_bson();
    //bool from_bson(const bson_t *doc);
    //bool from_json(const std::string &json_string);
    Value();
    Value(std::vector<T> v, bool fixed);
    Value(std::vector<T> v, bool fixed, Value *precursor);
    ~Value();

    std::vector<T> get_value();
    uint64_t get_time();
    void push_back(T v);
     */
};


#endif //CHINET_VALUE_H
