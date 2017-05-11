#pragma once
#include <stddef.h>
#include <stdint.h>
#include <std/cstdlib.hpp>

namespace std {
	template<class T>
	void swap(T x, T y);

	template<class T_in, class T_out>
	T_out copy(T_in first, T_in last, T_out d_first);
	
	template<class InputIt, class T>
	InputIt find(InputIt first, InputIt last, const T& value)
	{
		for (; first != last; ++first) {
			if (*first == value) {
				return first;
			}
		}
		return last;
	}

	template<class InputIt, class T>
	InputIt find(InputIt first, InputIt last, T& value)
	{
		for (; first != last; ++first) {
			if (*first == value) {
				return first;
			}
		}
		return last;
	}

	template<class InputIt, class UnaryPredicate>
	InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q)
	{
		for (; first != last; ++first) {
			if (!q(*first)) {
				return first;
			}
		}
		return last;
	}

	template<class InputIt, class UnaryPredicate>
	InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
	{
		for (; first != last; ++first) {
			if (p(*first)) {
				return first;
			}
		}
		return last;
	}

	template<class InputIt, class UnaryPredicate>
	int count_if(InputIt first, InputIt last, UnaryPredicate p)
	{
		int n = 0;
		for (; first != last; ++first) {
			if (p(*first)) {
				++n;
			}
		}
		return n;
	}

	template<class InputIt, class OutputIt>
	OutputIt move(InputIt first, InputIt last, OutputIt d_first)
	{
		while (first != last) {
			*d_first++ = std::move(*first++);
		}
		return d_first;
	}

	template<class ForwardIt, class T>
	ForwardIt remove(ForwardIt first, ForwardIt last, const T& value)
	{
		first = std::find(first, last, value);
		if (first != last)
			for(ForwardIt i = first; ++i != last; )
				if (!(*i == value))
					*first++ = std::move(*i);
		return first;
	}

	template<class ForwardIt, class UnaryPredicate>
	ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p)
	{
		first = std::find_if(first, last, p);
		if (first != last)
			for(ForwardIt i = first; ++i != last; )
				if (!p(*i))
					*first++ = std::move(*i);
		return first;
	}

	// std::begin
	template< class C > 
	auto begin( C& c ) -> decltype(c.begin()) { return c.begin(); }
	template< class C > 
	auto begin( const C& c ) -> decltype(c.begin()) { return c.begin(); }
	// std::end
	template< class C > 
	auto end( C& c ) -> decltype(c.end()) { return c.end(); }
	template< class C > 
	auto end( const C& c ) -> decltype(c.end()) { return c.end(); }

	// std::for_each
	template<typename Container, typename Function>
	void for_each(Container&& cont, Function f) {
		using std::begin; // allows us to overload begin and end
		auto it = begin(cont);
		using std::end; // allows us to overload begin and end
		auto end_it = end(cont);

		while (it != end_it) {
			f(*it);
			++it;
		}
	}

	// std::min
	template <class T> const T& min (const T& a, const T& b) {
		return !(b<a)?a:b;
	}

	// std::max
	template <class T> const T& max (const T& a, const T& b) {
		return (a<b)?b:a;
	}
}