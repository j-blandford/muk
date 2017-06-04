#pragma once

#include <stddef.h>
#include <std.hpp>

namespace std {
	using streamsize = int;

	template<class CharT>
	class basic_streambuf {
	public:
		using char_type = CharT;
		using pos_type = std::size_t;
		using int_type = int;

		~basic_streambuf();
		basic_streambuf();
		basic_streambuf(const basic_streambuf& rhs);

    	basic_streambuf<CharT>& operator=(const basic_streambuf<CharT>& rhs);
	protected:
		char_type* g_beg;
		char_type* g_cur;
		char_type* g_end;

		char_type* p_beg;
		char_type* p_cur;
		char_type* p_end;

	//	virtual int_type underflow(); // does nothing here. needs overriding

	public:
		char_type* eback() const { return g_beg; }
		char_type* gptr() const { return g_cur; }
		char_type* egptr() const { return g_end; }
		void gbump(int n) { g_cur += n; }
		void setg(char_type* gbeg, char_type* gnext, char_type* gend);

		std::streamsize in_avail() { return egptr() - gptr(); }

		int_type sgetc() { return (int_type)(*g_cur); }
		int_type snextc() { g_cur++; return (int_type)(*g_cur); }
		int_type sbumpc() { g_cur++; return (int_type)(*(g_cur-1)); }

		std::streamsize sgetn( char_type* s, std::streamsize count );

		char_type* pbase() const { return p_beg; }
		char_type* pptr() const { return p_cur; }
		char_type* epptr() const { return p_end; }
		void pbump(int n) { p_cur += n; }
		void setp(char_type* pbeg, char_type* pend);

		std::streamsize sputn( const char_type* s, std::streamsize count );
	};

	using streambuf = std::basic_streambuf<char>;
}