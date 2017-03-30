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

void schedule_next(struct registers * r) {
	if(!init_base_r) {
		memcpy(&base_reg, r, sizeof(registers));
		init_base_r = true;
	}

	if(!thread_list[cTask]->ran) {
		memcpy(&thread_list[cTask]->state_reg, &base_reg, sizeof(registers));
		thread_list[cTask]->state_reg.eip = (uint32_t)thread_list[cTask]->entry_ptr;
		thread_list[cTask]->ran = true;
	}
	
	int lastThread = cTask - 1;

	if(cTask == 0)
		lastThread = thread_list.size()-1;

	// save the previous threads state
	memcpy(&thread_list[lastThread]->state_reg, r, sizeof(registers));

	// set the registers from the current thread's saved state
	memcpy(r, &thread_list[cTask]->state_reg, sizeof(registers));

	cTask++;

	// Loop
	if(cTask >= thread_list.size()) {
		cTask = 0;
	}
	
}

void scheduler_init() {
	set_isr_handler(0x82, scheduler_isr130);
}