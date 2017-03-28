#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>

#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/thread.hpp>
#include <kernel/proc/process.hpp>


static int cTask = 0;

static void scheduler_isr130(struct registers * r) {
	schedule_next(r);
	MAGIC_BREAK;
	return;
}

static void task_idle() {
	for(;;) 
		__asm__ volatile("sti;hlt");
}

void set_ip (uintptr_t eip) {
	__asm__ __volatile__ ( "call %0 \n" : : "r"(eip) );
}

void schedule_next(struct registers * r) {
	if(cTask >= thread_list.size()) {
		cTask = 0;
	}

	//MAGIC_BREAK;

	// BochsConsolePrintChar( cTask+'0');

	thread_list[cTask]->r = r;
	thread_list[cTask]->r->eip = (uint32_t)thread_list[cTask]->entry_ptr;

	//	set_ip(thread_list[cTask]->r->eip);

	//	MAGIC_BREAK;

	cTask++;

	
}

void scheduler_init() {
	set_isr_handler(0x82, scheduler_isr130);
}