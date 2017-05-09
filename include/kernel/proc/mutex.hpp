#pragma once

#include <kernel/cpu.hpp>

namespace Process {
	template<class Lockable>
	class Locker {
		Lockable& lock;
		volatile int l_threadid;
	public:
		Locker(Lockable& mutex, int thread_id)
		: lock(mutex)
		, l_threadid(thread_id) { 
			lock.Lock(l_threadid); 
		}
		~Locker() { 
			lock.Unlock(l_threadid); 
		}
	};

	class SpinlockMutex {
		volatile int interlock;
		volatile int l_threadid;

	public:
		SpinlockMutex() 
		: interlock(0) { }

		~SpinlockMutex() { };

		void Lock(int thread_id);
		void Unlock(int thread_id);
		bool TryLock(int thread_id);
		bool Test();
		void Wait();

		template<typename> friend class Locker;
	};

	class RecursiveMutex {
		volatile int interlock;
		int l_threadid;

	public:
		RecursiveMutex() 
		: interlock(0) { }

		~RecursiveMutex() { };

		void Lock(int thread_id);
		void Unlock(int thread_id);
		bool TryLock(int thread_id);
		bool Test();
		void Wait();

		template<typename> friend class Locker;
	};
}