#pragma once

#include <stddef.h>

#include <std.hpp>
#include <std/vector.hpp>

#include <kernel/tty.hpp>
#include <kernel/user/env.hpp>
#include <kernel/proc/thread.hpp>

class Command {
public:
    static std::vector<std::string> split(char* command);
    static void Parse(char * buffer);
};