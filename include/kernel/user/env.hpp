#pragma once

#include <stddef.h>

#include <std.hpp>
#include <std/vector.hpp>

class pair {
public:
    char* key;
    string value;
    pair() { }
    pair(char* key, string value) : key(key), value(value) { }
};

namespace ENV {
    extern std::vector<pair> environment;

    string get(char* envVar);
    void set(string envVar, string envVal);
    
    void initialise();
};