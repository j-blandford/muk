
#include <kernel/memory/heap.hpp>
#include <kernel/cpu.hpp>

// our heap goes from V.addr 0xE0000000 -> 0xF0000000
static uintptr_t kheap_top = KERNEL_HEAP_START;
static uintptr_t kheap_start = 0; // our metadata

void expand_heap(uintptr_t start, size_t size) {
    bcprintf("expand_heap -------------------->\n");
    while(start+size >= kheap_top) {
        bcprintf("    kheap_top=%x\n",kheap_top);

        map_vaddr_page(kheap_top);

        memset((void*)kheap_top, 0, PAGE_SIZE);

        // allocate as many pages as necessary!
        kheap_top += PAGE_SIZE;
    }
}

// kernel heap memory allocation
void* kmalloc(size_t size) {
    bcprintf("kmalloc(size=%d) kheap_top=%x\n",size,kheap_top);

    bcprintf("    kheap_start=%x\n",kheap_start);

    if(!kheap_start) {
        // we need to initialise the heap!
        map_vaddr_page(KERNEL_HEAP_START);
        memset((void*)KERNEL_HEAP_START, 0, PAGE_SIZE);

        kheap_start = KERNEL_HEAP_START;

        bcprintf(" >>>>>>>>>   initialised heap\n");
    }

    BlockHeader* found_block = (BlockHeader*)kheap_start;
    for(; found_block->next != 0 && found_block->next != found_block->prev; found_block = found_block->next) {
        bcprintf("        looking for block: %x -> \n",found_block); 
    }

    bcprintf("          found_block=%x\n",found_block);
    bcprintf("          found_block->size=%d\n",found_block->size);
    bcprintf("          found_block->prev=%x\n",found_block->prev);
    bcprintf("          found_block->next=%x\n",found_block->next); 

    size += sizeof(BlockHeader);
    // We've found a free block (unitialised), let's set it up :)
    expand_heap((uintptr_t)found_block + found_block->size, size);

    BlockHeader* next_block = (BlockHeader*)(found_block + found_block->size);

    next_block->next = 0;
    next_block->prev = found_block;
    next_block->in_use = true;
    next_block->size = size;

    if(found_block->in_use) 
        found_block->next = found_block;

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
