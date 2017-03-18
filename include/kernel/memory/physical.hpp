#pragma once

#include <kernel/multiboot.hpp>

void pmm_setup(multiboot_info_t *mboot);

uintptr_t pmm_allocate();
void pmm_free(uintptr_t page);
