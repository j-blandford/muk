#include <kernel/memory/physical.hpp>

#define PAGE_DIRECTORY_OFFSET_BITS 10
#define PAGE_TABLE_OFFSET_BITS 10
#define PAGE_OFFSET_BITS 12

#define BITMAP_SIZE 4096	// enough for 4096*8 = 32mb of RAM

uint32_t pg_num_free;
uint32_t pg_free_bmp[BITMAP_SIZE];

/**
* bitmap sets the correct bit in the array to 1
*/ 
void pg_mark_free(uint32_t pg_num) {
	pg_free_bmp[pg_num >> 5] |= (1 << (pg_num & 0x1F));
	pg_num_free++;
}

/**
* bitmap sets the correct bit in the array to 0
*/ 
void pg_mark_taken(uint32_t pg_num) {
	pg_free_bmp[pg_num >> 5] &= ~(1 << (pg_num & 0x1F));
	pg_num_free--;
}

/**
*	the heart of the PMM - we say "hey, give me a page!" and it returns a free address.
*	very handy abstraction
*/ 
void * page_allocate() {
	for(size_t chunk = 0; chunk < BITMAP_SIZE; chunk++) {
		if(pg_free_bmp[chunk] != 0) {
			// there is a free page to find
			for(uint8_t bit = 0; bit < 32; bit++) {
				if((pg_free_bmp[chunk] & (1 << bit)) != 0) {
					// there's a free page here, let's mark it TAKEN
					uint32_t pg_num = chunk*32 + bit;

					pg_mark_taken(pg_num);
					return (void*)(pg_num << PAGE_OFFSET_BITS);
				}
			}
		}
	}

	return NULL; // no free memory was found :(
}

/**
* 	Function which initialises the memory bitmap from the GRUB boot information
*/
void pmm_setup(multiboot_info_t* mboot, uint32_t k_phys_start, uint32_t k_phys_end) {
	terminal_printf("[INFO] Total memory:  %d kb\n",mboot->mem_upper + mboot->mem_lower);

	pg_num_free = 0;

	// GRUB provides the kernel with a struct containing the map of memory
	// let's parse this to get a list of free physical memory pages
	memory_map_t * mmap = (memory_map_t*)(mboot->mmap_addr + KERNEL_VIRT_BASE);
	size_t mmap_entries = mboot->mmap_length / sizeof(memory_map_t);

	terminal_printf("[PMM] Physical memory layout is: ");
	for(size_t index = 0; index < mmap_entries; index++) {

		if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			// the memory page is free! let's mark each bit of the bitmap "free".
			// as each mem page is larger than our PMM page size, we need to individually 
			// loop over the mmap's memory space and mark each as "free"

			uint32_t first_pg = (mmap->base_addr_low & 0xFFFFF000) >> PAGE_OFFSET_BITS; // 4kb-aligned page mask with a bit-shift
			uint32_t end_pg = ((mmap->base_addr_low + mmap->length_low) & 0xFFFFF000) >> PAGE_OFFSET_BITS; // 4kb-aligned page mask with a bit-shift

			for(uint32_t pg_index = first_pg; pg_index < end_pg; pg_index++) {
				pg_mark_free(pg_index);
			}

			// Print how many pages we have in this chunk
			terminal_printf("(%d)", mmap->length_low / PAGE_SIZE);
		}

		mmap ++;
	}

	terminal_printf("\n");

	uint32_t end_kernel_pg = (k_phys_end & 0xFFFFF000) >> PAGE_OFFSET_BITS;
	for(uint32_t pg_index = 0; pg_index < end_kernel_pg+1; pg_index++) {
		pg_mark_taken(pg_index);
	}

	//for(uint32_t )
	//pg_mark_taken(0);
}

/**
*	Converts a page index into it's virtual address so that we can alter specific pages
*/
void* pg_virtual_addr(uint16_t pg_num) {
  uint32_t virt_addr = 0xFFC00000; // 0b1111111111

  // Next 10 bits pg index
  virt_addr |= (pg_num << PAGE_OFFSET_BITS);

  return (void*) virt_addr;
}

/**
* 	Sets up recursive page directory to allow us to change the PDEs during runtime
*/
page_directory_t pg_directory_setup() {
	const int KERNEL_PAGE_NUMBER = 768;

	page_directory_t page_dir = (page_directory_t)&PDVirtualAddress; // PDVirtualAddress is defined in boot.s

	uint32_t recursive_pde = (uint32_t)&PDPhysicalAddress;
	recursive_pde |= (1);			// PRESENT
	recursive_pde |= (1 << 1);		// READ/WRITE
	recursive_pde |= (1 << 2);		// ALL ACCESS
	recursive_pde |= (1 << 5);		// CACHE DISABLE
	page_dir[1023] = recursive_pde; //** last page directory points to itself (which is virtual address 0xFFFFF000) **//

	terminal_printf("[PGT] Recursive paging successful\n");

	//terminal_printf("[PGT] Kernel page table installed\n");

	return page_dir;
}

// grabs the first bit of the entry - both PDEs and PTEs
inline bool is_present(uint32_t page_entry) {
	return page_entry & 0x1; 
}

/**
*	This is the heart of the memory manager - it bridges the physical memory manager (which uses
*	a bitmap) and the page table manager together. This means we can just map as many virtual addresses
*   as we want (assuming we have enough memory to do so), as the PMM takes care of ALL physical addresses 
*    for us!
*/
void map_vaddr_page(uint32_t virtual_address) {
	page_directory_t page_dir = (page_directory_t)&PDVirtualAddress;

	uint32_t pd_index = virtual_address >> 22;
	uint32_t pde = page_dir[pd_index];

	if(!is_present(pde)) { // is the pg dir entry present?
		// NO! Let's set it up
		uint32_t* pde_phys_addr = (uint32_t*)page_allocate();

		uint32_t pde = (uint32_t)pde_phys_addr;
		pde |= (1);			// PRESENT
		pde |= (1 << 1);		// READ/WRITE
		pde |= (1 << 2);		// ALL ACCESS
		pde |= (1 << 5);		// CACHE DISABLE

		page_dir[pd_index] = pde; 

		// this is a new PD, we need to zero out the whole directory
		page_table_t page_table = (page_table_t)pg_virtual_addr(pd_index);
		for(int i = 0; i < 1024; i++) {
			page_table[i] = 0;
		}

		//bcprintf("pg: %d (%x), ", pd_index, pde_phys_addr);
	}

	page_table_t page_table = (page_table_t)pg_virtual_addr(pd_index); // uses recursive pg tables

	uint32_t pt_index = (virtual_address >> PAGE_OFFSET_BITS) & 0x03FF;
	uint32_t pte = page_table[pt_index];

	if(!is_present(pte)) { // is the pg table entry present?
		// NO! Let's set it up
		uint32_t* pte_phys_addr = (uint32_t*)page_allocate();

		uint32_t pte_new = (uint32_t)pte_phys_addr;
		pte_new |= (1);			// PRESENT
		pte_new |= (1 << 1);		// READ/WRITE
		pte_new |= (1 << 2);		// ALL ACCESS

		page_table[pt_index] = pte_new;

		//bcprintf("%d (%x)\n", pt_index,pte_phys_addr );
	}

	 
}