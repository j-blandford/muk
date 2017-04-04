
#include <kernel/memory/heap.hpp>
#include <kernel/cpu.hpp>

// our heap goes from V.addr 0xE0000000 -> 0xF0000000
static uintptr_t kheap_top = KERNEL_HEAP_START;
static mblock_t *kheap_start = 0; // our metadata

void expand_heap(uintptr_t start, size_t size) {
    while(start+size >= kheap_top) {
        map_vaddr_page(kheap_top);

        memset((void*)kheap_top, 0, PAGE_SIZE);

        // allocate as many pages as necessary!
        kheap_top += PAGE_SIZE;
    }
}

// kernel heap memory allocation
void* kmalloc(size_t size) {
    bcprintf("kmalloc(size=%d) kheap=%x\n",size,kheap_top);

    size += sizeof(mblock_t); // append the header metadata to each alloc block

    mblock_t* prev_block = kheap_start;
    mblock_t* free_block = kheap_start;

    bcprintf("    kheap_start=%x\n",kheap_start);

    while(free_block) {
        // if(!free_block->in_use) { // do some block splitting here
        //     return free_block
        // }

        // traverse the linked list to find the next NULL block (unitialised)
        prev_block = free_block;
        free_block = free_block->next;
    }

    if(!prev_block) {
        kheap_start = free_block = (mblock_t *)KERNEL_HEAP_START;
    }

    bcprintf("        free_block=%x\n",free_block);

    // We've found a free block (unitialised), let's set it up :)
    expand_heap((uintptr_t)free_block, size);

    free_block->next = 0;
    free_block->prev = prev_block;
    free_block->in_use = true;
    free_block->size = size;

    if(prev_block) 
        prev_block->next = free_block;

    return (void*)(free_block + sizeof(mblock_t));
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
