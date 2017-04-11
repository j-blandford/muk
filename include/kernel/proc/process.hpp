#pragma once

#include <std/vector.hpp>

#include <kernel/idt.hpp>
#include <kernel/proc/thread.hpp>
#include <kernel/memory/physical.hpp>

class Process {
public:
	size_t proc_id;
	char title[255];

	PageDirectory pd;

//	std::vector<Thread *> threads;

	registers r;

	Process() { }
	~Process() { }
};

//extern std::vector<Process *> proc_list;

void init_process();

void start_process(char* title);