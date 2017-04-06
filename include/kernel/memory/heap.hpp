#pragma once

#include <stdint.h>
#include <stddef.h>

#include <kernel/memory/alloc.hpp>
#include <kernel/memory/physical.hpp>

// each block of memory allocated has a small header of metadata
class BlockHeader {
public:
	bool in_use;
	uint32_t size;
	BlockHeader* next;
	BlockHeader* prev;

	BlockHeader() : in_use(true), size(0), next(0), prev(0) {}
};

void* kmalloc(size_t size);
void* kcalloc(size_t size);
void kfree(void* addr);
