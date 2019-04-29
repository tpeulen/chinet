//
// Created by thomas on 4/23/19.
//

#ifndef CHINET_SESSION_H
#define CHINET_SESSION_H

#include <mongoc.h>
#include <iostream>


class Session {

private:
    // Database stuff
    //------------------
    mongoc_uri_t *uri;
    mongoc_client_t *client;
    mongoc_collection_t *nodes, *ports, *links, *port_values;
    bson_error_t error;

public:
    bool connect_to_uri(const char* uri_string);
    Session() = default;
    Session(const char* uri_string);

};


#endif //CHINET_SESSION_H
