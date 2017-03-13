#pragma once

#include <stddef.h>
#include <stdint.h>

struct gdt_info {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__ ((packed));
typedef struct gdt_info gdt_info;

struct gdtr {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed));
typedef struct gdtr gdtr;

extern gdt_info gdt[5];
extern gdtr gp;

void gdt_install();
