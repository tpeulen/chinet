//
// Created by thomas on 4/23/19.
//

#include "Session.h"


//
//bool Session::connect_to_uri(const char* uri_string){
//    mongoc_init();
//
//    // Database
//    //----------------------------------------------------------------
//    std::clog << "connecting:" << uri_string << ":";
//    uri = mongoc_uri_new_with_error (uri_string, &error);
//    if (!uri) {
//        std::cerr << "failed to parse URI:" << uri_string << std::endl;
//        std::cerr << "error message:       " << error.message << std::endl;
//        return false;
//    } else{
//        /*
//        * Create a new client instance
//        */
//        client = mongoc_client_new_from_uri (uri);
//        /*
//        * Register the application name so we can track it in the profile logs
//        * on the server. This can also be done from the URI (see other examples).
//        */
//        mongoc_client_set_appname (client, "chinet-node");
//        /*
//         * Get a handle on the collections
//         */
//        node_collection = mongoc_client_get_collection (client, "db_chinet", "nodes");
//        port_collection = mongoc_client_get_collection (client, "db_chinet", "ports");
//        port_values = mongoc_client_get_collection (client, "db_chinet", "values");
//        port_links = mongoc_client_get_collection (client, "db_chinet", "links");
//        return true;
//    }
//}
//
