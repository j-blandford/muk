#include <kernel/memory/manager.hpp>
#include <kernel/cpu.hpp>

using namespace Memory;

BitmapAllocator<4096> Memory::phys_manager;
PageTableManager Memory::virt_manager;

/**
* 	Function which initialises the memory bitmap from the GRUB boot information
*/
void Memory::Setup(multiboot_info_t* mboot, uint32_t kernel_end) {
	phys_manager = BitmapAllocator<4096>(mboot, kernel_end);
	virt_manager = PageTableManager(&PDVirtualAddress, &PDPhysicalAddress); // PDVirtualAddress is defined in boot.s
}

/**
* bitmap sets the correct bit in the array to 1
*/ 
template<int BitmapSize>
void BitmapAllocator<BitmapSize>::PageFree(uint32_t page_index) {
	bitmap[page_index >> 5] |= (1 << (page_index & 0x1F));
	free_pages++;
}

/**
* bitmap sets the correct bit in the array to 0
*/ 
template<int BitmapSize>
void BitmapAllocator<BitmapSize>::PageTaken(uint32_t page_index) {
	bitmap[page_index >> 5] &= ~(1 << (page_index & 0x1F));
	free_pages--;
}

/**
*	the heart of the PMM - we say "hey, give me a page!" and it returns a free address.
*	very handy abstraction
*/ 
template<int BitmapSize>
void* BitmapAllocator<BitmapSize>::AllocatePage() {
	for(size_t chunk = 0; chunk < BitmapSize; chunk++) {
		if(bitmap[chunk] != 0) {
			// there is a free page to find
			for(uint8_t bit = 0; bit < 32; bit++) {
				if((bitmap[chunk] & (1 << bit)) != 0) {
					// there's a free page here, let's mark it TAKEN
					uint32_t pg_num = chunk*32 + bit;

					PageTaken(pg_num);
					return (void*)(pg_num << 12);
				}
			}
		}
	}

	return NULL; // no free memory was found :(
}

template<int BitmapSize>
BitmapAllocator<BitmapSize>::BitmapAllocator(multiboot_info_t *mboot, uint32_t kernel_end) : free_pages(0) {
	// GRUB provides the kernel with a struct containing the map of memory
	// let's parse this to get a list of free physical memory pages
	memory_map_t* mmap = (memory_map_t*)(mboot->mmap_addr + Memory::kVirtualBase);
	size_t mmap_entries = mboot->mmap_length / sizeof(memory_map_t);

	for(size_t index = 0; index < mmap_entries; index++) {
		if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			// the memory page is free! let's mark each bit of the bitmap "free".
			// as each mem page is larger than our PMM page size, we need to individually 
			// loop over the mmap's memory space and mark each as "free"

			uint32_t first_pg = (mmap->base_addr_low & 0xFFFFF000) >> 12; // 4kb-aligned page mask with a bit-shift
			uint32_t end_pg = ((mmap->base_addr_low + mmap->length_low) & 0xFFFFF000) >> 12; // 4kb-aligned page mask with a bit-shift

			for(uint32_t pg_index = first_pg; pg_index < end_pg; pg_index++) {
				PageFree(pg_index);
			}
		}
		mmap ++;
	}

	uint32_t end_kernel_pg = (kernel_end & 0xFFFFF000) >> 12;
	for(uint32_t pg_index = 0; pg_index < end_kernel_pg+1; pg_index++) {
		PageTaken(pg_index);
	}
}

// grabs the first bit of the entry - both PDEs and PTEs
static inline bool is_present(uint32_t page_entry) {
	return page_entry & 0x1; 
}

/**
* 	Sets up recursive page directory to allow us to change the PDEs during runtime
*/
PageTableManager::PageTableManager(uint32_t* VirtualAddress, uint32_t* PhysicalAddress) {
	page_dir = (PageDirectory)VirtualAddress; // PDVirtualAddress is defined in boot.s

	uint32_t recursive_pde = (uint32_t)PhysicalAddress;
	recursive_pde |= (1);			// PRESENT
	recursive_pde |= (1 << 1);		// READ/WRITE
	recursive_pde |= (1 << 2);		// ALL ACCESS
	recursive_pde |= (1 << 5);		// CACHE DISABLE
	page_dir[1023] = recursive_pde; //** last page directory points to itself (which is virtual address 0xFFFFF000) **//
}

/**
*	Converts a page index into it's virtual address so that we can alter specific pages
*/
PageTable PageTableManager::GetFromVirtualAddress(uint16_t page_index) {
	uint32_t virt_addr = Memory::kPageTableAddr;

	// Next 10 bits pg index
	virt_addr |= (page_index << 12);

	return reinterpret_cast<PageTable>(virt_addr);
}

/**
*	This is the heart of the memory manager - it bridges the physical memory manager (which uses
*	a bitmap) and the page table manager together. This means we can just map as many virtual addresses
*   as we want (assuming we have enough memory to do so), as the PMM takes care of ALL physical addresses 
*    for us!
*/
void PageTableManager::MapPage(uint32_t virtual_address, uint32_t physical_address) {
	uint32_t pd_index = virtual_address >> 22;
	uint32_t pt_index = (virtual_address >> 12) & 0x03FF;

	uint32_t pde = page_dir[pd_index];
	if(!is_present(pde)) { // is the pg dir entry present?
		// NO! Let's set it up
		uint32_t pde_phys_addr = (uint32_t)phys_manager.AllocatePage();

		uint32_t pde = pde_phys_addr;
		pde |= (1);			// PRESENT
		pde |= (1 << 1);		// READ/WRITE
		pde |= (1 << 2);		// ALL ACCESS
		pde |= (1 << 5);		// CACHE DISABLE

		page_dir[pd_index] = pde; 

		// this is a new PD, we need to zero out the whole directory
		PageTable page_table = GetFromVirtualAddress(pd_index);
		for(int i = 0; i < 1024; i++) {
			page_table[i] = 0;
		}
	}

	PageTable page_table = GetFromVirtualAddress(pd_index);

	uint32_t pte = page_table[pt_index];
	if(!is_present(pte)) { // is the pg table entry present?
		// NO! Let's set it up
		uint32_t pte_phys_addr = (physical_address == 0) ? (uint32_t)phys_manager.AllocatePage() : physical_address;

		uint32_t pte_new = pte_phys_addr;
		pte_new |= (1);			// PRESENT
		pte_new |= (1 << 1);		// READ/WRITE
		pte_new |= (1 << 2);		// ALL ACCESS

		page_table[pt_index] = pte_new;
	}
}