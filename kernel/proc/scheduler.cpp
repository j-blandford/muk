#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>

#include <kernel/memory/alloc.hpp>

#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/thread.hpp>
#include <kernel/proc/process.hpp>


static int cTask = 0;

static registers base_reg;
static bool init_base_r = false;

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
	__asm__ __volatile__ ( "jmp %0 \n" : : "r"(eip) );
}

void schedule_next(struct registers * r) {
	
	MAGIC_BREAK;

	bcprintf("\n>>>>>>>>>>>>>>>>> schedule_next()");

	bcprintf("\ncTask: %d (eip=%x)\n", cTask, r->eip);

	if(!init_base_r) {
		memcpy(&base_reg, r, sizeof(registers));
		init_base_r = true;
	}

	if(!thread_list[cTask]->ran) {
		memcpy(&thread_list[cTask]->state_reg, &base_reg, sizeof(registers));
		thread_list[cTask]->state_reg.eip = (uint32_t)thread_list[cTask]->entry_ptr;
	}

	bcprintf("task[-1]->eip: %x\n", r->eip);
	bcprintf("task[]->eip: %x\n", thread_list[cTask]->state_reg.eip);
	
	// Set the general process registers
	r->eax = thread_list[cTask]->state_reg.eax;
	r->edx = thread_list[cTask]->state_reg.edx;

	// we also need to set eflags for loop continuations
	r->eflags = thread_list[cTask]->state_reg.eflags;

	// set the instruction pointers directly (maybe unsafe.. look into this)
	r->eip = thread_list[cTask]->state_reg.eip;
	r->esp = thread_list[cTask]->state_reg.esp;

	thread_list[cTask]->ran = true;

	// LOOK HERE:>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// try set_ip so we dont corrupt the instrunction pointer!!!!
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	cTask++;

	if(cTask >= thread_list.size()) {
		cTask = 0;
	}

	bcprintf("<<<<<<<<<<<<<<<<< schedule_next()\n");

	//MAGIC_BREAK;
	
}

void scheduler_init() {
	set_isr_handler(0x82, scheduler_isr130);
}