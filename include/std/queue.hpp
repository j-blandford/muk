#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <std/ctype.hpp>
#include <std/string.hpp>

namespace std {
	// this is a shoddy way of implementing a queue. 
	// TODO: Implement a "deque" with multiple, fixed-sized arrays and then
	// derive this normal queue from the deque class.
	template<class T>
	class queue {
		vector<T> elements;
	public:
		using iterator = typename vector<T>::iterator;

		queue() = default;
		~queue() { }

		iterator begin() { return elements.begin(); }
		iterator end() { return elements.end(); }
		
		size_t size() const { return elements.size(); }

		void push(T value);
		void pop();
		T& peek();

		T& operator[](unsigned int index);
		const T& operator[](unsigned int index) const;
	};

	template<class T>
	void queue<T>::push(T value) {
		elements.push_back(value);
	}
	template<class T>
	void queue<T>::pop() {
		//elements.pop_back();
	}

	template<class T>
	T& queue<T>::operator[](unsigned int index) {
		return elements[index];
	}
}