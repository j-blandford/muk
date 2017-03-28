#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>
#include <kernel/timer.hpp>

#include <kernel/proc/process.hpp>
#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/thread.hpp>

#include <kernel/memory/physical.hpp>
#include <kernel/memory/heap.hpp>
#include <kernel/memory/alloc.hpp>

std::vector<Thread *> thread_list = std::vector<Thread *>();

static int next_tid = 1;

static void test_thread() {
	for(;;) {
		BochsConsolePrintChar('a');
	}
		//terminal_printf("1");
}

static void test_thread2() {
	for(;;) {
		BochsConsolePrintChar('b');
	}
		//terminal_printf("2");
}

/**
*  This is the hub of the process list. For the moment, this function is only called
* 	ONCE - for the "kernel" process. Every new thread is attached to this initial
*   process for the time being.
*/
void start_thread(char* title, void_fn entry) {
	Thread *thread = new Thread();
	memcpy(thread->title, title, strlen(title));
	thread->thread_id = next_tid;

	thread->entry_ptr = entry;

	thread_list.push_back(thread);

	next_tid++;
}

void init_kthreads() {
	start_thread("test", &test_thread);
	start_thread("test2", &test_thread2);

	terminal_writestring("Kernel Threads:\n");

	for(auto x : thread_list) {
		terminal_printf("  > '%s' @ %x\n", x->title, &x->entry_ptr);
	}
	//proc_list[0]
}