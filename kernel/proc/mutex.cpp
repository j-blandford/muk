#include <kernel/proc/mutex.hpp>

using namespace Process;

void SpinlockMutex::Lock(int thread_id) {
	while(interlock) { }

	this->l_threadid = thread_id;
	this->interlock = 1;

	bcprintf("Locked mutex\n");
}

void SpinlockMutex::Unlock(int thread_id) {
	if(this->l_threadid == thread_id) {
		this->interlock = 0;
		this->l_threadid = 0;
		bcprintf("Unlocked mutex\n");
	}
}

bool SpinlockMutex::TryLock(int thread_id) {
	if(this->l_threadid == 0) {
		this->Lock(thread_id);
		return true;
	} 
	else {
		return false;
	}
}

bool SpinlockMutex::Test() {
	return this->interlock;
}

void SpinlockMutex::Wait() {
	while(!this->Test()) { }
}


void RecursiveMutex::Lock(int thread_id) {
	this->l_threadid = thread_id;
	this->interlock++;
}

void RecursiveMutex::Unlock(int thread_id) {
	if(this->l_threadid == thread_id && this->interlock > 1) {
		this->interlock--;
		this->l_threadid = 0;
	}
}

bool RecursiveMutex::TryLock(int thread_id) {
	if(this->l_threadid == thread_id || this->l_threadid == 0) {
		this->Lock(thread_id);
		return true;
	} 
	else {
		return false;
	}
}

bool RecursiveMutex::Test() {
	return this->interlock;
}

void RecursiveMutex::Wait() {
	while(!this->Test()) { }
}