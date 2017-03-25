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

struct gdtr {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed));

struct tss_info {
	uint32_t ptl;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t u[15];
	uint32_t es, cs, ss, ds, fs, gs;
	uint32_t ldt;
	uint16_t trap, iomap;
}__attribute__((packed));

extern gdt_info gdt[6];
extern gdtr gp;

extern tss_info tss;

extern "C" void gdt_flush(gdtr* gp); // ASM function
extern "C" void tss_flush(uint32_t segment); // ASM funct

void gdt_install();
void tss_install();