#pragma once

#include <stddef.h>
#include <std.hpp>
#include <std/ios.hpp>

namespace std {
	template<class CharT>
	class basic_ostream : virtual public std::basic_ios<CharT> {

	};
}