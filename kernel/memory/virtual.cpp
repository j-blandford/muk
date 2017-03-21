#include <stddef.h>

#include <kernel/multiboot.hpp>
#include <kernel/tty.hpp>
#include <kernel/memory/alloc.hpp>
#include <kernel/memory/virtual.hpp>
#include <kernel/memory/physical.hpp>

uintptr_t* page_dir_ptr = reinterpret_cast<uintptr_t*>(PMM_PAGE_DIR);
uintptr_t* page_table_ptr = reinterpret_cast<uintptr_t*>(PMM_PAGE_TABLES);

uintptr_t vmm_pg_get(uintptr_t page) {
	uintptr_t page_idx = page & PAGE_PTR_MASK;

	if(page_dir_ptr[page_idx << 22] & 0x1)
		if(page_table_ptr[page_idx << 11] & 0x1) 
			return page_table_ptr[page_idx << 11];
	
	return NULL;
}

void vmm_pg_init(uintptr_t page, uint32_t flags) {
	page = page << 22;

	page_dir_ptr[page] = pmm_allocate() | flags;
	memset(reinterpret_cast<void*>(&page_dir_ptr[page]), 0, PAGE_SIZE);
}

void vmm_pg_set(uintptr_t page, uintptr_t page_data) {
	uint32_t flags = page_data & PAGE_FLAGS_MASK;
	uintptr_t page_idx = page & PAGE_PTR_MASK;

	if(!(page_dir_ptr[page_idx << 22] & 0x1))
		vmm_pg_init(page_idx, flags);
	
	page_table_ptr[page_idx << 12] = page_data;

	__asm__ volatile ("invlpg (%0)" : : "a" (page_idx));
}