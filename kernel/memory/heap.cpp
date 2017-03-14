#include <stdint.h>
#include <stddef.h>

#include <kernel/memory/alloc.hpp>
#include <kernel/memory/heap.hpp>

static void *kheap = &end;
static const void *heap_start = &end;

// memory allocation
void *malloc(size_t size) {
    kheap+=size;
    return kheap-size;
}

// re-allocate memory
void *realloc(void * ptr, size_t size) {
    if(ptr < heap_start)
    {
        return malloc(size);
    }
    else
    {
        void* newmem = malloc(size);
        memmove(newmem, ptr, size);
        free(ptr);
        return newmem;
    }
}

// alloc and zero memory
void *calloc(size_t num, size_t size) {
    size *= num;
    void* result = malloc(size);
    memset(result, 0, size);
    return result;
}

void free(void * ptr) {
    return; // meh for now
}
