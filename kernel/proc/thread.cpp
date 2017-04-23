#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>
#include <kernel/timer.hpp>
#include <kernel/drivers/keyboard.hpp>
#include <kernel/gfx/surface.hpp>

#include <kernel/proc/process.hpp>
#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/thread.hpp>
#include <kernel/proc/message.hpp>

#include <kernel/memory/manager.hpp>
#include <kernel/memory/heap.hpp>
#include <kernel/memory/alloc.hpp>

std::vector<Thread *> thread_list = std::vector<Thread *>();

static int next_tid = 1;

static void test_thread() {
	for(;;) {
		BochsConsolePrintChar('a');
	}
}
/**
*  This is the hub of the process list. For the moment, this function only adds 
*  threads to the "kernel" process. Every new thread is attached to this initial
*   process for the time being.
*/
void start_thread(char* title, void_fn entry) {
	Thread* thread = new Thread();
	memcpy(thread->title, title, strlen(title)+1);
	thread->thread_id = next_tid;

	thread->entry_ptr = entry;
	// thread->stack_ptr = (uintptr_t)kmalloc(4096); // 4kib of local thread stack

	thread->state_reg.eip = (uint32_t)entry;
	//thread->state_reg.esp = thread->stack_ptr;

	thread_list.push_back(thread);

	next_tid++;
}

void init_kthreads() {
	start_thread("keyboard_driver", &kb_update); // adds to the ringbuffer
	start_thread("tty_driver", &tty_update); // updates the terminal text and parses commands
	start_thread("display_driver", &surface_update); // keeps the frontbuffer in sync with the backbuffer
	start_thread("postbox_debug", &postbox_debug);
}