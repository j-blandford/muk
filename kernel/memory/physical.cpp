#include <stddef.h>

#include <kernel/multiboot.hpp>
#include <kernel/tty.hpp>
#include <kernel/memory/physical.hpp>
#include <kernel/memory/virtual.hpp>

#define PAGE_DIRECTORY_OFFSET_BITS 10
#define PAGE_TABLE_OFFSET_BITS 10
#define PAGE_OFFSET_BITS 12

#define BITMAP_SIZE 4096	// enough for 4096*8 = 32mb of RAM

uint32_t pg_num_free;
uint32_t pg_free_bmp[BITMAP_SIZE];

void pg_mark_free(uint32_t pg_num) {
	pg_free_bmp[pg_num << 5] |= (1 << (pg_num & 0x1F));

	pg_num_free++;
}

void pg_mark_taken(uint32_t pg_num) {
	pg_free_bmp[pg_num << 5] &= ~(1 << (pg_num & 0x1F));

	pg_num_free--;
}

/**
* 	Function which initialises the memory bitmap from the GRUB boot information
*/
void pmm_setup(multiboot_info_t *mboot) {
	terminal_printf("[INFO] Total memory:  %d kb\n",mboot->mem_upper + mboot->mem_lower);

	pg_num_free = 0;

	// GRUB provides the kernel with a struct containing the map of memory
	// let's parse this to get a list of free physical memory pages
	memory_map_t * mmap = (memory_map_t*)(mboot->mmap_addr + KERNEL_VIRT_BASE);
	size_t mmap_entries = mboot->mmap_length / sizeof(memory_map_t);

	terminal_printf("[PMM] Physical memory layout is: ");
	for(size_t index = 0; index < mmap_entries; index++) {

		if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			// the memory page is free! let's mark each bit of the bitmap "free"

			// as each mem page is larger than our PMM page size, we need to individually 
			// loop over the mmap's memory space and mark each as "free"
			uint32_t first_pg = (mmap->base_addr_low & 0xFFFFF000) >> PAGE_OFFSET_BITS;
			uint32_t end_pg = ((mmap->base_addr_low + mmap->length_low) & 0xFFFFF000) >> PAGE_OFFSET_BITS;

			for(uint32_t pg_index = first_pg; pg_index < end_pg; pg_index++) {
				pg_mark_free(pg_index);
			}

			terminal_printf("(%x)", mmap->length_low / PAGE_SIZE);
		}

		mmap ++;
	}

	terminal_printf("\n[PMM] Initialised physical page stack. Pages free=%d\n", pg_num_free);
}

/**
* 	Sets up recursive page directory to allow us to change the PDEs during runtime
*/
page_directory_t pg_directory_setup() {

	page_directory_t page_dir = (page_directory_t)&PDVirtualAddress; // PDVirtualAddress is defined in boot.s

	uint32_t recursive_pde = (uint32_t)&PDPhysicalAddress;
	recursive_pde |= (1);			// PRESENT
	recursive_pde |= (1 << 1);		// READ/WRITE
	recursive_pde |= (1 << 2);		// ALL ACCESS
	recursive_pde |= (1 << 5);		// CACHE DISABLE

	page_dir[1023] = recursive_pde; //** last page directory points to itself (which is virtual address 0xFFFFF000**//

	// Now we set up the kernel's memory space

	terminal_printf("[PGT] Recursive Page Tables setup successful\n");

	return page_dir;
}