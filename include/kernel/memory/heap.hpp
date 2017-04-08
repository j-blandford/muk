#pragma once

#include <stdint.h>
#include <stddef.h>

#include <kernel/memory/alloc.hpp>
#include <kernel/memory/physical.hpp>

// each block of memory allocated has a small header of metadata
// this is a linked list
class BlockHeader {
public:
	bool in_use;	// this field isn't used for the moment
	uint32_t size;
	BlockHeader* next; // ptr to the next block header
	BlockHeader* prev; // "" previous block header

	BlockHeader(size_t size, BlockHeader* prev) 
	: in_use(true)
	, size(size)
	, next(0)
	, prev(prev) { }
	
	BlockHeader(size_t size, BlockHeader* prev, BlockHeader* next)
	: in_use(true)
	, size(size)
	, next(next)
	, prev(prev) { }
};

void* kmalloc(size_t size);
void* kcalloc(size_t size);
void kfree(void* addr);
