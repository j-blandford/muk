#pragma once


#define USER_STACK_BEGIN 0xB0000000
#define USER_STACK_END 0xC0000000

#define THREAD_STACK_SIZE 4096

#include <std/vector.hpp>
#include <kernel/idt.hpp>

using void_fn = void(*)(void);

class Thread {
public:
	//registers_t * r;
	size_t thread_id;
	char *title;

	void_fn entry_ptr;
	registers state_reg;
	bool ran;

	Thread() : title(new char[255]), ran(false) {}
};

extern std::vector<Thread *> thread_list;

void start_thread(char* title);

void init_kthreads();