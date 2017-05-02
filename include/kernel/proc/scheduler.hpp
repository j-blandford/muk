#pragma once

#include <kernel/multiboot.hpp>
#include <kernel/gdt.hpp>
#include <kernel/idt.hpp>

namespace Scheduler {
	extern unsigned int lock_count;
	extern volatile bool running;
	
	void next(registers* r);
	bool lock();
	bool unlock();
	void yield();

	int threadId();
	void init();
};