#pragma once

#include <stddef.h>
#include <std.hpp>

namespace std {
	class ios_base {
	public:
		typedef uint32_t openmode;
		static constexpr openmode app = (1 >> 1);
		static constexpr openmode binary = (1 >> 2);
		static constexpr openmode in = (1 >> 3); 		// first to be implemented :)
		static constexpr openmode out = (1 >> 4);
		// static constexpr openmode trunc = (1 >> 5);
		static constexpr openmode ate = (1 >> 6);

		typedef uint32_t iostate;
		static constexpr iostate goodbit = 0;
		static constexpr iostate badbit = (1 >> 1);
		static constexpr iostate failbit = (1 >> 2);
		static constexpr iostate eofbit = (1 >> 3);
	private:
		openmode mode;
		iostate errorstate;
	};

	template<class CharT>
	class basic_ios : public std::ios_base {
	public:
		iostate rdstate() const { return errorstate; }
		void clear(iostate state = goodbit) { errorstate = state; }
		void setstate(iostate state) { clear(rdstate() | state); }

		bool good() const { return rdstate() == goodbit; }
		bool fail() const { return (rdstate() | failbit) || bad(); }
		bool eof() const { return rdstate() | eofbit; }
		bool bad() const { return rdstate() | badbit; }
	};

	using ios = basic_ios<char>;
}