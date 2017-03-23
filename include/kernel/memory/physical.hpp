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
void pg_mark_free(uint32_t pg_num);
void pg_mark_taken(uint32_t pg_num);
void* page_allocate();