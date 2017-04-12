#pragma once

#include <stddef.h>
#include <stdint.h>

#include <kernel/multiboot.hpp>
#include <kernel/tty.hpp>

// following variables are defined in asm/boot.s
extern uint32_t PDVirtualAddress;
extern uint32_t PDPhysicalAddress;

namespace Memory {	
	typedef uint32_t* PageDirectory;
	typedef uint32_t* PageTable;

	const uintptr_t kVirtualBase = 0xC0000000; // Our Higher-half kernel is loaded from this address
	const uintptr_t kHeapStart = 0xE0000000; // Our kernel's heap base virtual addres
		
	const int kNumPageEntries = 1024; // the number of entries per page directory (1024 for x86)
	const int kPageSize = 4096; // page size = 4 KiB
	const int kKernelPageIndex = ( kVirtualBase / kPageSize ) / kNumPageEntries; // kernel's pde

	// The following addresses require the use of a recursive page table
	const uintptr_t kPageTableAddr = 0xFFC00000;  // 0b1111111111
	const uintptr_t kPageDirAddr = 0xFFFFF000;

	class IAllocator {
	public:
		virtual ~IAllocator() { }
		virtual void PageFree(uint32_t page_index) = 0;
		virtual void PageTaken(uint32_t page_index) = 0;
		virtual void* AllocatePage() = 0;
	};

	template<int BitmapSize>
	class BitmapAllocator : public IAllocator {
		uint32_t free_pages;
		uint32_t bitmap[BitmapSize];
	public:
		BitmapAllocator() : free_pages(0) { }
		BitmapAllocator(multiboot_info_t* mboot, uint32_t kernel_end);	
		~BitmapAllocator() {}

		void PageFree(uint32_t page_index);
		void PageTaken(uint32_t page_index);
		void* AllocatePage();
	};

	class PageTableManager {
		PageDirectory page_dir; 
	public:
		PageTableManager() { }
		PageTableManager(uint32_t* VirtualAddress, uint32_t* PhysicalAddress);
		~PageTableManager() { }

		PageTable GetFromVirtualAddress(uint16_t page_index);
		void MapPage(uint32_t virtual_address, uint32_t physical_address = 0);
	};

	extern BitmapAllocator<4096> phys_manager;
	extern PageTableManager virt_manager;

	void Setup(multiboot_info_t* mboot, uint32_t kernel_end);
}