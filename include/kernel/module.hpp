#pragma once

#include <kernel/multiboot.hpp>

using void_fn = void(*)(void);

void_fn get_module_funct(multiboot_info_t * mb_info, int mdl_index);