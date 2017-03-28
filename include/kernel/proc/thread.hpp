#pragma once

#include <std/vector.hpp>
#include <kernel/idt.hpp>

using void_fn = void(*)(void);

class Thread {
public:
	registers_t * r;
	size_t thread_id;
	void_fn entry_ptr;
	char *title;

	bool ran;

	Thread() : ran(false), title(new char[255]) {}
};

extern std::vector<Thread *> thread_list;

void start_thread(char* title);

void init_kthreads();