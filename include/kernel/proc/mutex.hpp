#pragma once

namespace Process {
	class Spinlock {
		volatile int interlock;
		int l_threadid;

	public:
		Spinlock() 
		: interlock(0) { }

		~Spinlock();

		void Lock(int thread_id);
		void Unlock(int thread_id);
		bool Try();
		void Wait();

		friend class Locker;
	};

	class Locker {
		Spinlock& lock;
		int l_threadid;
	public:
		Locker(Spinlock& mutex, int thread_id)
		: lock(mutex)
		, l_threadid(thread_id) { 
			lock.Lock(l_threadid); 
		}
		~Locker() { 
			lock.Unlock(l_threadid); 
		}
	};
}