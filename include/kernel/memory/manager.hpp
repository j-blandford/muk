#pragma once

#include <stddef.h>
#include <stdint.h>

#include <kernel/multiboot.hpp>
#include <kernel/tty.hpp>

#define KERNEL_HEAP_START 0xE0000000

#define PMM_PAGE_TABLES 0xFFC00000
#define PMM_PAGE_DIR 0xFFFFF000
#define PMM_STACK_PTR 0xF0000000

#define PAGE_SIZE 4096

#define KERNEL_VIRT_BASE 0xC0000000
#define KERNEL_PAGE_IDX	(KERNEL_VIRT_BASE / PAGE_SIZE) / 1024 // for a higher half kernel, this = 768

extern uint32_t PDVirtualAddress;
extern uint32_t PDPhysicalAddress;

namespace Memory {
	typedef uint32_t* PageDirectory;
	typedef uint32_t* PageTable;

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
		PageTableManager(uint32_t VirtualAddress, uint32_t PhysicalAddress);
		~PageTableManager() { }

		PageTable GetFromVirtualAddress(uint16_t page_index);
		void MapPage(uint32_t virtual_address, uint32_t physical_address = 0);
	};

	extern BitmapAllocator<4096> phys_manager;
	extern PageTableManager virt_manager;

	void Setup(multiboot_info_t* mboot, uint32_t kernel_end);
}