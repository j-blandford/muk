#include <stddef.h>
#include <kernel/cpu.hpp>
#include <kernel/memory/heap.hpp>

void *operator new(size_t size) noexcept {
    return vmalloc(size);
}

void *operator new[](size_t size) noexcept {
    return vmalloc(size);
}

// size-unaware deallocation functions
void operator delete(void *p) noexcept {
    kfree(p);
}

void operator delete[](void *p) noexcept {
    kfree(p);
}

// size-aware deallocation functions (for C++14 -> S18.4 [support.dynamic])
void operator delete(void *p, size_t sz) noexcept {
    kfree(p, sz);
}

void operator delete[](void *p, size_t sz) noexcept {
    kfree(p, sz);
}

// Note: as we use -fno-exceptions, there are only TWO new C++14 deallocation
// functions!
