#pragma once

#define KERNEL_VIRT_BASE 0xC000000

#define PMM_PAGE_TABLES 0xFFC00000
#define PMM_PAGE_DIR 0xFFFFF000
#define PMM_STACK_PTR 0xF0000000

#define PAGE_SIZE 4096
#define PAGE_PTR_MASK 0xFFFFF000
#define PAGE_FLAGS_MASK 0xFFF
#define STACK_PTR_PER_PAGE 0x400

uintptr_t vmm_pg_get(uintptr_t page);

void vmm_pg_init(uintptr_t page, uint32_t flags);
void vmm_pg_set(uintptr_t page, uintptr_t page_data);