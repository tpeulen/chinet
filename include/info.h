#ifndef CHINET_INFO_H
#define CHINET_INFO_H

#include <cstdlib>
#include <string>

#define CHINET_VERSION             "0.0.11"
//#define CHINET_VERBOSE true

// Function to check if CHINET_VERBOSE environment variable is set
inline bool is_chinet_verbose() {
    const char* env = std::getenv("CHINET_VERBOSE");
    return env != nullptr && std::string(env) != "";
}

#endif //CHINET_INFO_H
