#pragma once

namespace std {
		
	template<class T, T v>
	struct integral_constant {
		static constexpr T value = v;
		typedef T value_type;
		typedef integral_constant type; // using injected-class-name
		constexpr operator value_type() const noexcept { return value; }
		constexpr value_type operator()() const noexcept { return value; } //since c++14
	};

	using true_type = std::integral_constant<bool, true>;
	using false_type = std::integral_constant<bool, false>;


	// REMOVE CONST-VOLATILE 
	template< class T >
	struct remove_cv {
		typedef typename std::remove_volatile<typename std::remove_const<T>::type>::type type;
	};
	
	template< class T > struct remove_const          { typedef T type; };
	template< class T > struct remove_const<const T> { typedef T type; };
	
	template< class T > struct remove_volatile             { typedef T type; };
	template< class T > struct remove_volatile<volatile T> { typedef T type; };

	template< class T >
	using remove_cv_t = typename remove_cv<T>::type;

	template< class T >
	using remove_const_t = typename remove_const<T>::type;

	template< class T >
	using remove_volatile_t = typename remove_volatile<T>::type;
	// ^^^^^^^^^^^^^^^^^^^^^^

	// IS SAME generic template
	template<class T, class U>
	struct is_same : std::false_type {};
	
	template<class T>
	struct is_same<T, T> : std::true_type {};
	// ^^^^^^^^^^^^^^^^^^^^^^

	
	template< class T >
	struct is_void : std::is_same<void, typename std::remove_cv<T>::type> {};


}