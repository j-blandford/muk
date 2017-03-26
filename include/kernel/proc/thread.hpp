#pragma once

#include <kernel/idt.hpp>
#include <kernel/proc/process.hpp>

class Thread {
public:
	registers_t reg;
	size_t thread_id;
	Process *proc;
};