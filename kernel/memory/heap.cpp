#include <kernel/memory/heap.hpp>
#include <kernel/cpu.hpp>

// our heap goes from V.addr 0xE0000000 -> 0xF0000000
static uintptr_t kheap_top = Memory::kHeapStart;
static uintptr_t kheap_start = 0; // our metadata

// this takes care of the heap <-> Page table mappings so the heap doesn't have to worry
// about any of the underlying physical page allocation functions
void map_heap_pages(uintptr_t end_addr) {
    for(; end_addr > kheap_top; kheap_top += Memory::kPageSize) {
        Memory::virt_manager.MapPage(kheap_top);
        
        memset((void*)kheap_top, 0, Memory::kPageSize);        
    }
}

// kernel heap memory allocation
void* kmalloc(size_t size) {
    bcprintf("> kmalloc(%d)\n",size);

    if(size == 0) {
        bcprintf("KERNEL ERROR: Cannot allocate 0 bytes!\n");
        return 0;
    }

    if(!kheap_start) {
        // we need to initialise the heap!
        Memory::virt_manager.MapPage(Memory::kHeapStart);
        memset((void*)Memory::kHeapStart, 0, Memory::kPageSize);

        kheap_start = Memory::kHeapStart;
        kheap_top = Memory::kHeapStart;
    }

    // Lets traverse the linked list to find the next block which is free (has no "next" ptr)
    BlockHeader* found_block = (BlockHeader*)kheap_start;
    for(; found_block->next && found_block != found_block->next; found_block = found_block->next) {
        // bcprintf("     found_block=%x, \n", found_block);
        // bcprintf("        ->size=%x\n", found_block->size);
        // bcprintf("        ->next=%x\n", found_block->next);
    }

    size += sizeof(BlockHeader);

    uintptr_t end_of_block = (uintptr_t)found_block + found_block->size; // end of the last free block
    map_heap_pages(end_of_block + size); // see if we need to map pages to fit the block in

    BlockHeader* next_block = new ((void*)end_of_block) BlockHeader(size, found_block);// now the memory is 100% initialised, let's start using it!

    // Update the previous block to point to this memory block
    if(found_block) 
        found_block->next = next_block;

    // bcprintf("-> (%x)\n",(next_block + sizeof(BlockHeader)));

    return (void*)(next_block + sizeof(BlockHeader));
}

// alloc and zero memory
void* kcalloc(size_t size) {
    void* result = kmalloc(size);
    memset(result, 0, size);
    return result;
}

void kfree(void * addr) {
    return; // meh for now
}
