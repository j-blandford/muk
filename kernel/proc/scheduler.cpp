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
	bcprintf("\n>>>>>>>>>>>>>>>>> schedule_next()");

	bcprintf("\ncTask: %d\n", cTask);
	//thread_list[cTask]->r = r;

	// save the current instruction pointer
	if(cTask == 0) {
		if(thread_list[thread_list.size()-1]->ran) {
			thread_list[thread_list.size()-1]->eip = r->eip;
			bcprintf("       Setting cTask [%d]->eip=%x\n",thread_list.size()-1,r->eip);
		}
			
	} 
	else {
		if(thread_list[cTask-1]->ran) {
			thread_list[cTask-1]->eip = r->eip;
			bcprintf("       Setting cTask [%d]->eip=%x\n",cTask-1,r->eip);
		}
	}
	bcprintf("task[-1]->eip: %x\n", r->eip);
	bcprintf("task[]->eip: %x\n", thread_list[cTask]->eip);

	// set the instruction pointer directly (maybe unsafe.. look into this)
	r->eip = thread_list[cTask]->eip;
	thread_list[cTask]->ran = true;

	cTask++;

	if(cTask >= thread_list.size()) {
		cTask = 0;
	}

	bcprintf("<<<<<<<<<<<<<<<<< schedule_next()\n");

	MAGIC_BREAK;
	
}

void scheduler_init() {
	set_isr_handler(0x82, scheduler_isr130);
}