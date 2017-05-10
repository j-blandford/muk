#pragma once

#include <stddef.h>

#include <std.hpp>
#include <std/vector.hpp>

class pair {
public:
    char* key;
    std::string value;
    pair() { }
    pair(char* key, std::string value) : key(key), value(value) { }
};

namespace ENV {
    extern std::vector<pair> environment;

    std::string get(char* envVar);
    void set(std::string envVar, std::string envVal);
    
    void initialise();
};