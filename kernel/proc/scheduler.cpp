#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>
#include <kernel/timer.hpp>

#include <kernel/memory/alloc.hpp>

#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/thread.hpp>
#include <kernel/proc/process.hpp>

unsigned int Scheduler::task_idx(0);
unsigned int Scheduler::lock_count(0); 
bool Scheduler::has_initialised(false);
volatile bool Scheduler::running(true);
registers Scheduler::base_state;

// this function currently isn't used. I need to work out how
// to call interrupts from within the IRQ0 (timer) interrupt.
static void scheduler_isr130(registers * r) {
	Scheduler::next(r);
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

		return;
	}

	if(!thread_running->ran) {
		// if this is the first time the thread has been scheduled,
		// we need to initialize it's state register beforehand
		bcprintf("   . setting up thread (%d)\n",thread_running->thread_id);

		//memcpy((void*)&(thread_running->state_reg), &base_state, sizeof(registers)); 

		thread_running->state_reg.eflags = base_state.eflags;
		thread_running->state_reg.esp = base_state.esp;
		thread_running->state_reg.isr_num = 0;
		thread_running->state_reg.err_code = 0;

		thread_running->state_reg.eip = (uint32_t)thread_running->entry_ptr;
	
		thread_running->ran = true;
	} 
	else {
		// save the previous threads state
		memcpy((void*)&(thread_running->state_reg), r, sizeof(registers));
	}

	// Lets move on with task switching!
	thread_running = thread_running->next;

	if(thread_running->ran) {
		bcprintf("\nthread: %s (%d)", thread_running->title, thread_running->thread_id);

		// set the registers from the current thread's saved state
		memcpy(r, (void*)&(thread_running->state_reg), sizeof(registers));	
	}
}

// this is used to allow threads to perform blocking I/O calculations.
// it stops schedule_next from switching context to another thread until
// resume() is called. 
// Note: this can HANG THE KERNEL if used improperly!!!!!
bool Scheduler::lock() {
	if(running) {
		running = false;
		return true;
	}
	return false; // already locked, we cannot lock AGAIN!
}

bool Scheduler::unlock() {
	if(!running) {
		running = true;
		return true;
	}
	return false; // already unlocked, we cannot unlock AGAIN!
}

void Scheduler::yield() {
	Scheduler::unlock();
	Timer::yield();
}

int Scheduler::threadId() {
	return thread_running->thread_id;
}

void Scheduler::init() {
	set_isr_handler(0x82, scheduler_isr130); // currently not used
}