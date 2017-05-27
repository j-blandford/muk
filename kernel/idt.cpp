#include <stddef.h>
#include <stdint.h>

#include <kernel/cpu.hpp>
#include <kernel/tty.hpp>
#include <kernel/idt.hpp>

#include <kernel/memory/alloc.hpp>

static struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_p;

extern "C" void idt_flush(uint32_t);

static idt_entry_t idt_entries[256];

static void idt_set_gate(uint8_t i, uint32_t offset, uint16_t selector, uint8_t flags) {
	idt_entries[i].offset_low = offset & 0xFFFF;
	idt_entries[i].selector = selector;
	idt_entries[i].zero = 0;
	idt_entries[i].type_attr = flags & 0xFF;
	idt_entries[i].offset_high = (offset >> 16) & 0xFFFF;
}

void idt_install() {
	memset((void*)&idt_entries, 0x0, sizeof(struct idt_entry) * 256);

	// initialize the PIC
	outportb(PIC1_COMMAND, 0x11); // start initialisation on the master
	outportb(PIC2_COMMAND, 0x11); // and slave PIC

	outportb(PIC1_DATA, Interrupt::kPICRemap);
	outportb(PIC2_DATA, Interrupt::kPICRemap + 0x08);

	outportb(PIC1_DATA, 0x04); // setup cascading
	outportb(PIC2_DATA, 0x02);

	outportb(PIC1_DATA, 0x01);
	outportb(PIC2_DATA, 0x01);

	outportb(PIC1_DATA, 0x00); // enable interrupts 0-7
	outportb(PIC2_DATA, 0x00); // enable interrupts 8-15

	// Exceptions
	idt_set_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
	idt_set_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
	idt_set_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
	idt_set_gate( 3, (uint32_t)isr3 , 0x08, 0x8E);
	idt_set_gate( 4, (uint32_t)isr4 , 0x08, 0x8E);
	idt_set_gate( 5, (uint32_t)isr5 , 0x08, 0x8E);
	idt_set_gate( 6, (uint32_t)isr6 , 0x08, 0x8E);
	idt_set_gate( 7, (uint32_t)isr7 , 0x08, 0x8E);
	idt_set_gate( 8, (uint32_t)isr8 , 0x08, 0x8E);
	idt_set_gate( 9, (uint32_t)isr9 , 0x08, 0x8E);
	idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
	idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
	idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
	idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
	idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
	idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
	idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
	idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
	idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
	idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
	idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
	idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
	idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
	idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
	idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
	idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
	idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
	idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
	idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
	idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
	idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
	idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

	// IRQs
	idt_set_gate(32, (uint32_t)isr32, 0x08, 0x8E);
	idt_set_gate(33, (uint32_t)isr33, 0x08, 0x8E);
	idt_set_gate(34, (uint32_t)isr34, 0x08, 0x8E);
	idt_set_gate(35, (uint32_t)isr35, 0x08, 0x8E);
	idt_set_gate(36, (uint32_t)isr36, 0x08, 0x8E);
	idt_set_gate(37, (uint32_t)isr37, 0x08, 0x8E);
	idt_set_gate(38, (uint32_t)isr38, 0x08, 0x8E);
	idt_set_gate(39, (uint32_t)isr39, 0x08, 0x8E);
	idt_set_gate(40, (uint32_t)isr40, 0x08, 0x8E);
	idt_set_gate(41, (uint32_t)isr41, 0x08, 0x8E);
	idt_set_gate(43, (uint32_t)isr42, 0x08, 0x8E);
	idt_set_gate(44, (uint32_t)isr44, 0x08, 0x8E);
	idt_set_gate(45, (uint32_t)isr45, 0x08, 0x8E);
	idt_set_gate(46, (uint32_t)isr46, 0x08, 0x8E);
	idt_set_gate(47, (uint32_t)isr47, 0x08, 0x8E);

	for (int i = 49; i <= 255; i++) {
		// FIXME: create a special dummy handler
		idt_set_gate(i, (uint32_t)isr0, 0x08, 0x8E);
	}

	idt_set_gate(0x80, (uint32_t)isr128, 0x08, 0x8E);
	idt_set_gate(0x82, (uint32_t)isr130, 0x08, 0x8E); // SCHEDULE INTERRUPT

	idt_p.limit = sizeof(struct idt_entry) * 256 - 1;
	idt_p.base = (uint32_t)&idt_entries;

	idt_flush((uint32_t)&idt_p);
}

namespace Interrupt {
	const size_t kPICRemap = 0x20; // 20h = 32d for pic remapping offset
	isr_fn handlers[256];

	void Register(size_t irq_num, isr_fn ptr) {
		handlers[irq_num + kPICRemap] = ptr; // irqs are remapped +32
	}
}

extern "C" registers* isr_handler(registers* registers) {
	// first we send a ping back to the device (if it's an IRQ)
	if (registers->isr_num >= Interrupt::kPICRemap 
		&& registers->isr_num <= Interrupt::kPICRemap + 0x0F) { 
		// OK so the ISR corresponds to an IRQ event (ie. between 0 and 15)

		if (registers->isr_num >= Interrupt::kPICRemap + 0x08) {
			// as the PIC has 2 chips, 0-8 and 9-15, we need to send the EOI
			// packet to the second chip, too:
			outportb(PIC2_COMMAND, PIC_EOI);
		}

		outportb(PIC1_COMMAND, PIC_EOI);
	}

	if (Interrupt::handlers[registers->isr_num] != nullptr) {
		// now if a handler is registered for this interrupt, let's call it
		Interrupt::handlers[registers->isr_num](registers);
	}

	return registers;
}
