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

unsigned int Scheduler::lock_count(0); 
volatile bool Scheduler::running(true);

static bool started = false;

// this function currently isn't used. I need to work out how
// to call interrupts from within the IRQ0 (timer) interrupt.
static void scheduler_isr130(registers * r) {
	Scheduler::next(r);
}

// this is the heart of our time slicing algorithm. It's extremely
// basic at the moment, with only a single process allowed and no
// ability to skip or change the time slice itself.
void Scheduler::next(registers * r) {
	interrupts_disable();

	MAGIC_BREAK;

	if(started) {
		// save the previous threads state
		if(r->esp > 0xE0000000) {
			memcpy(&(thread_running->state_reg), r, sizeof(registers));
		}

		// Lets move on with task switching!
		thread_running = thread_running->next;

		switch_context(&(thread_running->prev->state_reg.esp),
						 &(thread_running->state_reg.esp));
	} 
	else {
		started = true;
		set_context(&(thread_running->stack_ptr));
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