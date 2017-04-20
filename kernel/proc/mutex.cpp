#include <kernel/proc/mutex.hpp>

using namespace Process;

void Spinlock::Lock(int thread_id) {
	while(interlock) { }

	this->l_threadid = thread_id;
	this->interlock = 1;
}

void Spinlock::Unlock(int thread_id) {
	if(this->l_threadid == thread_id)
		this->interlock = 0;
}

bool Spinlock::Try() {
	return this->interlock;
}

void Spinlock::Wait() {
	while(!this->Try()) { }
}