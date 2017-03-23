#pragma once

#include <kernel/multiboot.hpp>

typedef uint32_t * page_directory_t;
typedef uint32_t * page_table_t;

extern uint32_t PDVirtualAddress;
extern uint32_t PDPhysicalAddress;

// Page Table functions
page_directory_t pg_directory_setup();


// Page Allocation functions
void pmm_setup(multiboot_info_t *mboot);
uintptr_t pmm_allocate();
void pmm_free(uintptr_t page);
