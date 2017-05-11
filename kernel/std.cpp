#include <stddef.h>
#include <kernel/cpu.hpp>
#include <kernel/memory/heap.hpp>

void *operator new(size_t size) {
    return vmalloc(size);
}

void *operator new[](size_t size) {
    return vmalloc(size);
}

void operator delete(void *p) {
    kfree(p);
}

void operator delete[](void *p) {
    kfree(p);
}
