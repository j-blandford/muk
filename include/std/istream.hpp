#pragma once

#include <stddef.h>
#include <std.hpp>
#include <std/ostream.hpp>
#include <std/ios.hpp>

namespace std {
	template<class CharT>
	class basic_istream : virtual public std::basic_ios<CharT> {

	};

	template<class CharT>
	class basic_iostream : public std::basic_istream<CharT>, public std::basic_ostream<CharT>  {

	};
}