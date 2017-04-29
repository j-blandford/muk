#pragma once

#include <stddef.h>
#include <stdint.h>

inline void interrupts_disable() {
	__asm__ __volatile__ ("cli");
}
inline void interrupts_enable() {
	__asm__ __volatile__ ("sti");
}

typedef struct registers {
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;

	uint32_t edi; // 16
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx; // 36
	uint32_t ecx;
	uint32_t eax;

	uint32_t isr_num; //48
	uint32_t err_code;

	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t useresp;
	uint32_t ss;
} __attribute__((packed)) registers_t;

extern "C" void set_stack_ptr(uint32_t, uint32_t);

uint8_t inportb (uint16_t port);
void outportb (uint16_t port, uint8_t data);

uint16_t inports (uint16_t port);
void outports (uint16_t port, uint16_t data);

uint32_t inportl (uint16_t port);
void outportl (uint16_t port, uint32_t data);

__attribute__((noreturn)) void halt();

// Is a useless instruction. Causes Bochs to break at the calling point
#define MAGIC_BREAK asm volatile ("xchg %bx, %bx;");

//outputs a character to the debug console
inline void BochsConsolePrintChar(char c) {
	outportb(0xe9, (int)c);
}

//stops simulation and breaks into the debug console
inline void BochsBreak() {
	outportl(0x8A00,0x8A00); 
	outportl(0x8A00,0x08AE0);
} 

void BochsConsolePrint(char *s);
void bcprintf(const char* fmt, ...);

// this cannot be an inline function as __FILE__ and __LINE__ would be incorrect.
#define assert(n) ({if(!(n)){ \
	interrupts_disable(); \
	bcprintf("\n---------------------\n"); \
	bcprintf("HALT! \n Assertion failed -> %s\n", #n); \
	bcprintf("%s @ line %d", __FILE__, __LINE__-4); \
  	for(;;);}})

