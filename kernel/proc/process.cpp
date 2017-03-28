#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>

#include <kernel/proc/process.hpp>
#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/thread.hpp>

#include <kernel/memory/physical.hpp>

std::vector<Process *> proc_list = std::vector<Process *>();

static int next_pid = 1;

/**
*  This is the hub of the process list. For the moment, this function is only called
* 	ONCE - for the "kernel" process. Every new thread is attached to this initial
*   process for the time being.
*/
void start_process(char* title) {
	// Process *proc = new Process();
	// proc->proc_id = next_pid;
	// memcpy(proc->title, title, strlen(title));

	// Thread *main_thread = new Thread();
	// main_thread->pd = pg_dir;

	// proc->r = 

	// proc->threads.push_back(main_thread);
	// proc_list.push_back(proc);

	// next_pid++;
}

void init_process(page_directory_t kernel_pgdir) {
//	start_process("kernel", kernel_pgdir);

	//proc_list[0]
}