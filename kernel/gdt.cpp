#include <stddef.h>
#include <stdint.h>

#include <kernel/gdt.hpp>

gdt_info gdt[6]; // our table
gdtr gp;	// this is what points to our table. we point our CPU to this variable.

tss_info tss; // global TSS struct

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"

	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;

	#pragma GCC diagnostic pop
}

void gdt_install() {
	gp.limit = (sizeof(gdt_info) * 6) - 1;
	gp.base = (uint32_t) &gdt;

	gdt_set_gate(0, 0, 0, 0, 0); // first gate is ALWAYS null
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xC0); // 0x9A corresponds to ring 0 CODE memory segments
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xC0); // 0x92 corresponds to  ring 0 DATA memory segments
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xF8, 0xC0); // User Code (r3) segment maybe these 2 shouldn't overlap with the ring 0...
  	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xC0); // User Data (r3) segment

	gdt_flush(&gp);
}

void tss_install() {
	gdt_set_gate(5, (uint32_t)&tss, sizeof(tss_info), 0x89, 0x40); // TSS segment

	tss.ss0 = 0x10; // 0x10 = kernel data segment
	tss.iomap = sizeof(tss_info);

	tss_flush(0x28);
}