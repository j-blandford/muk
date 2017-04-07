
#include <kernel/memory/heap.hpp>
#include <kernel/cpu.hpp>

// our heap goes from V.addr 0xE0000000 -> 0xF0000000
static uintptr_t kheap_top = KERNEL_HEAP_START;
static uintptr_t kheap_start = 0; // our metadata

// this takes care of the heap <-> Page table mappings so the heap doesn't have to worry
// about any of the underlying physical page allocation functions
void map_heap_pages(uintptr_t end_addr) {
    for(; end_addr > kheap_top; kheap_top += PAGE_SIZE) {
        map_vaddr_page(kheap_top);
        memset((void*)kheap_top, 0, PAGE_SIZE);        
    }
}

// kernel heap memory allocation
void* kmalloc(size_t size) {
    bcprintf("> kmalloc(size=%d)\n",size);

    if(!kheap_start) {
        // we need to initialise the heap!
        map_vaddr_page(KERNEL_HEAP_START);
        memset((void*)KERNEL_HEAP_START, 0, PAGE_SIZE);

        kheap_start = KERNEL_HEAP_START;
    }

    // Lets traverse the linked list to find the next block which is free (has no "next" ptr)
    BlockHeader* found_block = (BlockHeader*)kheap_start;
    for(; found_block->next && found_block != found_block->next; found_block = found_block->next) { }

    size += sizeof(BlockHeader);

    uintptr_t end_of_block = (uintptr_t)found_block + found_block->size; // end of the last free block
    map_heap_pages(end_of_block + size); // see if we need to map pages to fit the block in

    BlockHeader* next_block = (BlockHeader*)end_of_block; // now the memory is 100% initialised, let's start using it!
    next_block->in_use = true;
    next_block->size = size;
    next_block->next = 0;
    next_block->prev = found_block;

    // Update the previous block to point to this memory block
    if(found_block) 
        found_block->next = next_block;

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
