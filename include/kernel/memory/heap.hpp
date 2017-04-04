#pragma once

#include <stdint.h>
#include <stddef.h>

#include <kernel/memory/alloc.hpp>
#include <kernel/memory/physical.hpp>

// each block of memory allocated has a small header of metadata
struct mblock_t {
	mblock_t* prev;
	mblock_t* next;
	bool in_use;
	uint32_t size;
} __attribute__((packed));

void* kmalloc(size_t size);
void* kcalloc(size_t size);
void kfree(void* addr);
