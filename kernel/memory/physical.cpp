#include <stddef.h>

#include <kernel/multiboot.hpp>
#include <kernel/tty.hpp>
#include <kernel/memory/physical.hpp>
#include <kernel/memory/virtual.hpp>

#define DEFAULT_PD_FLAGS 1; // "PRESENT" flag is only one set


/**
 * "pmm_*" functions 
 * 	You have a hunk of memory (located at PMM_STACK_PTR) which is initialised from the GRUB 
 * 	memory map. Each free page of memory is added to this stack and then just popped off as needed. 
 * 	We don't have to worry about the memory being discontinuous, as our VMM will map it so it looks
 * 	like one contiguous block of memory to the user processes.
 * 	Pointers for the stack are stored in the pages which are being used. The beauty of this is that 
 * 	we can disregard the memory size of the stack itself, as the stack cleans up after itself!
 */

uintptr_t pmm_curr;
uintptr_t* pmm_stack_ptr = reinterpret_cast<uintptr_t*>(PMM_STACK_PTR);
uintptr_t* pmm_stack_end_ptr = reinterpret_cast<uintptr_t*>(PMM_STACK_PTR);

void pmm_free(uintptr_t page) {
	page &= PAGE_PTR_MASK; // cuts off the last 3 bytes (0xFFF) which is used for the flags (also 4kb aligns the address)

	if(pmm_stack_ptr >= pmm_stack_end_ptr) { 
		// our stack isn't large enough, let's allocate more memory to accomodate this
		pmm_stack_end_ptr += STACK_PTR_PER_PAGE;
	}
	else {
		// an existing page has been freed, lets just push it to the stack
		pmm_stack_end_ptr = reinterpret_cast<uintptr_t*>(page);
		pmm_stack_end_ptr++;
	}
}

uintptr_t pmm_allocate() {
	pmm_stack_ptr--;

	if(pmm_stack_ptr < (pmm_stack_end_ptr - STACK_PTR_PER_PAGE)) {
		pmm_stack_end_ptr -= STACK_PTR_PER_PAGE;

		uintptr_t new_pg_ptr = vmm_pg_get(reinterpret_cast<uintptr_t>(pmm_stack_end_ptr) & PAGE_PTR_MASK);
		vmm_pg_set(new_pg_ptr, 0);
		return new_pg_ptr;
	}

	return *pmm_stack_ptr;
}

void pmm_setup(multiboot_info_t *mboot) {
	terminal_printf("[INFO] Starting PMM...\n");
	terminal_printf("[INFO] Total memory:  %d kb\n",mboot->mem_upper + mboot->mem_lower);

	pmm_curr = (mboot->mem_upper + PAGE_SIZE) & PAGE_PTR_MASK;

//	terminal_printf("[PMM] Physical page stack is at %x\n", pmm_stack_ptr);

	// GRUB provides the kernel with a struct containing the map of memory
	// let's parse this to get a list of free physical memory pages
	uintptr_t mmap_ptr = mboot->mmap_addr;
	uintptr_t mmap_end_ptr = mboot->mmap_addr + mboot->mmap_length;

	terminal_printf("[PMM] Physical memory layout is: ");

	// while(mmap_ptr < mmap_end_ptr) {
	// 	multiboot_mmap_entry_t *mmap = (multiboot_mmap_entry_t *)mmap_ptr;

	// 	if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
	// 		// the memory page is free! let's push this to the "free" stack page by page

	// 		// as each mem page is larger than our PMM page size, we need to individually 
	// 		// loop over the mmap's memory space and add each to the stack of free memory.

	// 		// for(uintptr_t pg_ptr = mmap->addr_low; pg_ptr < mmap->addr_low+mmap->length_low; pg_ptr += PAGE_SIZE) {
	// 		// 	pmm_free(pg_ptr); // add this page to our stack of free pages
	// 		// }

	// 		terminal_printf("(%x)", mmap->length_low / PAGE_SIZE);
	// 	}
		
	// 	mmap_ptr+= sizeof(multiboot_mmap_entry_t);
	// }

	terminal_printf("\n[PMM] Initialised physical page stack.\n");
}

page_directory_t pg_directory_setup() {
	terminal_printf("[PGT] PD Virtual address: %x\n", &PDVirtualAddress);
	terminal_printf("[PGT] PD Physical address: %x\n", &PDPhysicalAddress);

	page_directory_t page_dir = reinterpret_cast<page_directory_t>(&PDVirtualAddress); // PDVirtualAddress is defined in boot.s

	//terminal_printf("[PGT] page_dir[768]: %x\n", page_dir[768]);

	//						|phys addr  | pres|  r/w     |  access  | no cache|
	page_dir[1023] = &PDPhysicalAddress | (1) | (1 << 1) | (1 << 2) | (1 << 5);//*; // last page directory points to itself

	return page_dir;
}