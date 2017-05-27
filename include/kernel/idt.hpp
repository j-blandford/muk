#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kernel/cpu.hpp>

typedef struct idt_entry {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

void idt_install();

#define PIC1_COMMAND    0x20 // chip 1
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0 // chip 2 -> gives us 15 distinct IRQs (+1 for the zeroth)
#define PIC2_DATA       0xA1

#define PIC_EOI         0x20

namespace Interrupt {
	using isr_fn = void(*)(registers*);
	extern const size_t kPICRemap;
	extern isr_fn handlers[256];

	void Register(size_t irq_num, isr_fn ptr);
}

// exceptions
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

// IRQs
extern "C" void isr32(); // the timer irq (0)
extern "C" void isr33(); // the kb irq (1)
extern "C" void isr34();
extern "C" void isr35();
extern "C" void isr36();
extern "C" void isr37();
extern "C" void isr38();
extern "C" void isr39();
extern "C" void isr40();
extern "C" void isr41();
extern "C" void isr42();
extern "C" void isr43();
extern "C" void isr44();
extern "C" void isr45();
extern "C" void isr46(); // ata 0,0 (14)
extern "C" void isr47(); // ata 0,1 (15)

extern "C" void isr128(); // syscall (0x80, or 0x60 + 32)
extern "C" void isr130(); // interrupt scheduler