#pragma once

#include <stddef.h>

#include <std.hpp>
#include <std/vector.hpp>

class Command {
public:
    static std::vector<string> split(char* command);
    static void Parse(char * buffer);
};