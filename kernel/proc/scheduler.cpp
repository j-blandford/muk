#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>

#include <kernel/memory/alloc.hpp>

#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/thread.hpp>
#include <kernel/proc/process.hpp>

// Define the in-class private static members...
int Scheduler::task_idx(0);
bool Scheduler::has_initialised(false);
bool Scheduler::running(true);
registers Scheduler::base_state;

// this function currently isn't used. I need to work out how
// to call interrupts from within the IRQ0 (timer) interrupt.
static void scheduler_isr130(registers * r) {
	// scheduler_next(r);
	// MAGIC_BREAK;
	// return;
}

// this is the heart of our time slicing algorithm. It's extremely
// basic at the moment, with only a single process allowed and no
// ability to skip or change the time slice itself.
void Scheduler::next(registers * r) {
	if(!has_initialised) {
		// the first time our scheduler gets called, we need to
		// grab the initial registers so that each thread gets a 
		// copy upon being ran.
		memcpy(&base_state, r, sizeof(registers));
		has_initialised = true;
	}

	if(!thread_list[task_idx]->ran) {
		// if this is the first time the thread has been scheduled,
		// we need to initialize it's state register beforehand
		memcpy(&thread_list[task_idx]->state_reg, &base_state, sizeof(registers));
		thread_list[task_idx]->state_reg.eip = (uint32_t)thread_list[task_idx]->entry_ptr;

		thread_list[task_idx]->ran = true;
	}

	if(running) {
		int lastThread = task_idx - 1;

		if(task_idx == 0)
			lastThread = thread_list.size()-1;

		// save the previous threads state
		memcpy(&thread_list[lastThread]->state_reg, r, sizeof(registers));

		// set the registers from the current thread's saved state
		memcpy(r, &thread_list[task_idx]->state_reg, sizeof(registers));

		task_idx++;

		// Loop around 0 -> 1 -> 2 -> 0 -> ...
		if(task_idx >= thread_list.size()) {
			task_idx = 0;
		}
	}
}

// this is used to allow threads to perform blocking I/O calculations.
// it stops schedule_next from switching context to another thread until
// resume() is called. 
// Note: this can HANG THE KERNEL if used improperly!!!!!
bool Scheduler::pause() {
	if(running) {
		running = false;
		return true;
	}
	return false; // already paused, we cannot pause AGAIN!
}

bool Scheduler::resume() {
	if(!running) {
		running = true;
		return true;
	}
	return false; // already running, we cannot resume AGAIN!
}

void Scheduler::init() {
	set_isr_handler(0x82, scheduler_isr130); // currently not used
}