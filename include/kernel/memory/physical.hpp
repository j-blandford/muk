#pragma once

#include <kernel/multiboot.hpp>

void pmm_setup(mboot_info_t *mboot);

uintptr_t pmm_allocate();
void pmm_free(uintptr_t page);
