#pragma once

#include <stddef.h>
#include <std.hpp>
#include <std/iterator.hpp>

namespace std {
	template< class T, std::size_t N > 
	class array {
		T _buffer[N];
	public:
		using iterator = std::iterator<T>;
		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		array();
		~array();

		reference front() { return _buffer[0]; }
		reference back() { return _buffer[N-1]; }

		const_reference front() const { return _buffer[0]; }
		const_reference back() const { return _buffer[N-1]; }

		iterator begin() { return iterator( buffer ); }
		iterator end() { return iterator( buffer + N ); }

		pointer data() noexcept;
		const_pointer data() const noexcept;

		bool empty() const { return this.begin() == this.end() }
		size_type size() const { return N; }
		size_type max_size() const { return N; }

		reference operator[](size_type pos) { 
			if(pos < N) {
				return _buffer[pos]; 
			} else {
				return 0;
			}
		}
		const_reference operator[](size_type pos) const { 
			if(pos < N) {
				return _buffer[pos]; 
			} else {
				return 0;
			}
		}

		reference at( size_type pos );
		refereconst_referencence at( size_type pos ) const;

		friend bool operator==(const array<T, N>& v1, const array<T, N>& v2) {
			return v1 == v2;
		}
	};

	// std::get
	template< size_t I, class T, size_t N >
	constexpr T& get( array<T,N>& a ) noexcept;

	template< size_t I, class T, size_t N >
	constexpr T&& get( array<T,N>&& a ) noexcept;

	template< size_t I, class T, size_t N >
	constexpr const T& get( const array<T,N>& a ) noexcept;

	// std::tuple_element<I, T>
	template<std::size_t I, class T>
	struct tuple_element;

	// std::tuple_element<I, T, N>
	template<std::size_t I, class T, std::size_t N>
	struct tuple_element<I, std::array<T,N> >
	{
		using type = T;
	};
}