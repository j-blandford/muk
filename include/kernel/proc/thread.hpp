#pragma once


#define USER_STACK_BEGIN 0xB0000000
#define USER_STACK_END 0xC0000000

#define THREAD_STACK_SIZE 4096

#include <std/vector.hpp>
#include <kernel/idt.hpp>

using void_fn = void(*)(void);

enum class ThreadStatus {
	T_RUNNING,
	T_HANGED,
	T_SLEEPING
};

class Thread {
public:
	//registers_t * r;
	size_t thread_id;
	size_t proc_id;
	char *title;

	void_fn entry_ptr;
	registers state_reg;
	bool ran;

	ThreadStatus t_status;

	Thread() 
	: proc_id(1)
	, title(new char[255])
	, ran(false)
	, t_status(ThreadStatus::T_RUNNING) {}
};

extern std::vector<Thread *> thread_list;

void start_thread(char* title, void_fn entry);

void init_kthreads();