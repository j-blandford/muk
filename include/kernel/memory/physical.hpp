#pragma once

#include <kernel/multiboot.hpp>

#define PMM_PAGE_TABLES 0xFFC00000
#define PMM_PAGE_DIR 0xFFFFF000
#define PMM_STACK_PTR 0xF0000000

#define PAGE_SIZE 4096

#define KERNEL_VIRT_BASE 0xC0000000
#define KERNEL_PAGE_IDX	(KERNEL_VIRT_BASE / PAGE_SIZE) / 1024 // for a higher half kernel, this = 768

typedef uint32_t * page_directory_t;
typedef uint32_t * page_table_t;

extern uint32_t PDVirtualAddress;
extern uint32_t PDPhysicalAddress;

// Page Table functions
page_directory_t pg_directory_setup();


// Page Allocation (bitmap) functions
void pmm_setup(multiboot_info_t *mboot, uint32_t k_phys_start, uint32_t k_phys_end);
void pg_mark_free(uint32_t pg_num);
void pg_mark_taken(uint32_t pg_num);
void* page_allocate();