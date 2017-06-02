#pragma once

#include <stddef.h>
#include <std.hpp>
#include <std/streambuf.hpp>
#include <std/istream.hpp>

namespace std {
	template<class CharT>
	class basic_filebuf : public std::basic_streambuf<CharT> {
		using char_type = CharT;
	public:
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

		basic_filebuf() { }
		~basic_filebuf();

		bool is_open() const;
		basic_filebuf<char_type>* open(const string& filename, ios_base::openmode mode);
		basic_filebuf<char_type>* open(const char* filename, ios_base::openmode mode);
		basic_filebuf<char_type>* close();
	};

	using filebuf = basic_filebuf<char>;

	template<class CharT> // class TraitsType = std::char_traits<CharT>
	class basic_fstream : public std::basic_iostream<CharT> {
		using char_type = CharT;
		basic_filebuf<char_type> sb;
	public:
		basic_fstream() { }
		explicit basic_fstream(const string& path, ios_base::openmode mode = ios_base::in|ios_base::out);
		explicit basic_fstream(const char *path, ios_base::openmode mode = ios_base::in|ios_base::out);

		void open(const string& filename, ios_base::openmode mode = ios_base::in|ios_base::out) {
			// rdbuf()->open(filename, mode);
		}
		void open(const char *filename, ios_base::openmode mode = ios_base::in|ios_base::out) {
			// rdbuf()->open(filename, mode);
		}

		bool is_open() { return rdbuf()->is_open(); }
		bool is_open() const { return rdbuf()->is_open(); }

		basic_filebuf<char_type>* rdbuf() const { return &sb; };
	};

	using fstream = basic_fstream<char>;
}