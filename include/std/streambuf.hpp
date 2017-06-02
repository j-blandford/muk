#pragma once

#include <stddef.h>
#include <std.hpp>

namespace std {
	template<class CharT>
	class basic_streambuf {
	public:
		using char_type = CharT;
		using pos_type = std::size_t;
		using int_type = int;

		virtual ~basic_streambuf();

	protected:
		basic_streambuf() { }
		basic_streambuf(const basic_streambuf& rhs);
    	basic_streambuf& operator=(const basic_streambuf& rhs);

		char_type* eback() const;
		char_type* gptr() const;
		char_type* egptr() const;
		void gbump(int n);
		void setg(char_type* gbeg, char_type* gnext, char_type* gend);

		char_type* pbase() const;
		char_type* pptr() const;
		char_type* epptr() const;
		void pbump(int n);
		void setp(char_type* pbeg, char_type* pend);
	};

	using streambuf = std::basic_streambuf<char>;
}