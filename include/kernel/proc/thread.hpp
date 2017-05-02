#pragma once


#define USER_STACK_BEGIN 0xB0000000
#define USER_STACK_END 0xC0000000

#define THREAD_STACK_SIZE 4096

#include <std/vector.hpp>
#include <kernel/idt.hpp>

using void_fn = void(*)();

enum class ThreadStatus {
	T_RUNNING,
	T_HANGED,
	T_SLEEPING
};

class Thread {
public:
	size_t thread_id;
	size_t proc_id;
	char *title;

	registers state_reg;

	uintptr_t entry_ptr;
	uintptr_t stack_ptr;

	Thread* prev;
	Thread* next;

	uint32_t stack[THREAD_STACK_SIZE];

	ThreadStatus t_status;

	Thread() 
	: proc_id(1)
	, title(new char[255])
	, prev(nullptr)
	, next(nullptr)
	, t_status(ThreadStatus::T_RUNNING) {}
};

extern Thread* thread_root;
extern Thread* thread_running;

void start_thread(char* title, void_fn entry);

void init_kthreads();