#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct buffer {
	size_t head;
	size_t tail;
	size_t size;
	char * buffer;
} buffer_t;

int buffer_read(buffer_t* buffer);
int buffer_write(buffer_t* buffer, uint8_t v);
int buffer_clear(buffer_t* buffer);

namespace std {
	template<class CharType>
	class ringbuffer {
		using size_type = size_t;
		using pointer = CharType*;
		using reference = CharType&;

		pointer buffer = nullptr;
		size_type _capacity = 0;

		size_type head = 0;
		size_type tail = 0;
	public:
		ringbuffer() = delete;

		ringbuffer(size_type capacity)
		: buffer(new CharType[size])
		, _capacity(capacity) { }

		ringbuffer(const ringbuffer<CharType> &rhs)
		: buffer(new CharType[rhs._capacity])
		, _capacity(rhs._capacity) {
			// // need to make this function leak-proof...
			// std::copy(rhs.buffer, rhs.buffer + rhs._capacity, buffer);
		}

		~ringbuffer() { delete[] buffer; }

		const reference operator[](size_type idx) const;
		reference operator[](size_type idx); 

		size_type capacity() const { return _capacity; }
		size_type size() const { return tail - head; }
		bool empty() const { return (head == tail); }
		void swap(ringbuffer<CharType>& rhs) {
			// std::swap(buffer,rhs.buffer);
			// std::swap(_capacity, rhs._capacity);
			// std::swap(head, rhs.head);
			// std::swap(tail, rhs.tail);
		}

		friend void swap(ringbuffer<CharType>& lhs, ringbuffer<CharType>& rhs) {
    		lhs.swap(rhs);
		}

		ringbuffer<CharType>& operator <<(CharType c);
		ringbuffer<CharType>& operator <<(CharType* c);
	};
}