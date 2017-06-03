#pragma once

#include <stdint.h>
#include <stddef.h>

#include <kernel/memory/alloc.hpp>
#include <kernel/memory/manager.hpp>

// each block of memory allocated has a small header of metadata
// this is a linked list
class BlockHeader {
public:
	uint32_t in_use : 1;	// this field isn't used for the moment
	uint32_t size : 31;
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

void* vmalloc(size_t size);
void* kcalloc(size_t size);
void kfree(void* addr);
void kfree(void* addr, size_t sz); // for c++14 compatibility

void* malloc(size_t size);
void* calloc(size_t size);
void* calloc(int num, size_t size);
void* realloc(void* addr, size_t new_size);
void free(void* addr);
void free(void* addr, size_t sz);