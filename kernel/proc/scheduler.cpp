#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>

#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/thread.hpp>

// static void scheduler_isr130(struct registers * registers) {
// 	return;
// }

static void task_idle() {
	for(;;) 
		__asm__ volatile("sti;hlt");
}

void schedule() {
	BochsConsolePrintChar('t');
}

void scheduler_init() {
//	set_isr_handler(0x82, scheduler_isr130);
}