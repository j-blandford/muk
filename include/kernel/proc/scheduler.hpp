#pragma once

#include <kernel/multiboot.hpp>
#include <kernel/gdt.hpp>
#include <kernel/idt.hpp>

#define set_kstack(stack) tss.esp0 = (uint32_t)(stack)

class Scheduler {
	static int task_idx;
	static registers base_state;
	static bool has_initialised;
	static bool running;
	
public:
	static void next(registers* r);
	static bool lock();
	static bool unlock();
	static void yield();
	
	static void init();
};