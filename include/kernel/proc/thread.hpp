#pragma once


#define USER_STACK_BEGIN 0xB0000000
#define USER_STACK_END 0xC0000000

#define THREAD_STACK_SIZE 16384

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
	char* title;

	registers state_reg;

	uintptr_t entry_ptr;
	uintptr_t stack_ptr;

	Thread* prev = nullptr;
	Thread* next = nullptr;

	uint32_t stack[THREAD_STACK_SIZE];

	ThreadStatus t_status = ThreadStatus::T_RUNNING;

	Thread() 
	: proc_id(1)
	, title(new char[255]) {}
};

extern Thread* thread_root;
extern Thread* thread_running;

void start_thread(char* title, void_fn entry);

void init_kthreads();