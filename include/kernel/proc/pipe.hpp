#pragma once

#include <stddef.h>
#include <stdint.h>

#include <std/algorithm.hpp>
#include <std/ringbuffer.hpp>
#include <kernel/proc/mutex.hpp>
#include <kernel/proc/thread.hpp>

namespace Process {

	// struct Node {

	// };

	// this class allows two threads to communicate directly
	template<class CharType>
	class Pipe {
		using size_type = typename std::ringbuffer<CharType>::size_type;
		std::ringbuffer<CharType> buffer;
		std::string name;
		volatile SpinlockMutex locks[2];
	public:
		Pipe() = delete;
		Pipe(std::string pipename, Thread* t1, Thread* t2, size_type max_size) 
		: buffer(std::ringbuffer<CharType>(max_size))
		, name(pipename) { 
			// t1->stdout = this;
			// t2->stdin = this;
		}

		Pipe& operator<<(CharType c);
		Pipe& operator<<(CharType* c);

		CharType pop();
		bool push(CharType c);

		~Pipe() { }
	};
}