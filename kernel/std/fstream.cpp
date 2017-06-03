#include <std/fstream.hpp>

#include <kernel/tty.hpp>

namespace std {
	
	/**********************************************
	* basic_filebuf<CharT>
	**********************************************/

	template class basic_filebuf<char>; // we need to provide a concrete implementation to link to

	template<class CharT>
	basic_filebuf<CharT>* basic_filebuf<CharT>::open(const string& filename, ios_base::openmode mode) {
		return nullptr;
	}

	template<class CharT>
	basic_filebuf<CharT>* basic_filebuf<CharT>::open(const char* filename, ios_base::openmode mode) {
		return nullptr;
	}

	template<class CharT>
	basic_filebuf<CharT>* basic_filebuf<CharT>::close() {

	}

	template<class CharT>
	bool basic_filebuf<CharT>::is_open() const {
		return true;
	}

	// ctor
	template<class CharT>
	basic_filebuf<CharT>::basic_filebuf() {
	}
	// dtor
	template<class CharT>
	basic_filebuf<CharT>::~basic_filebuf() {
		if(is_open()) 
			close();
	}

	/**********************************************
	* basic_fstream<CharT>
	**********************************************/

	template class basic_fstream<char>;

	template<class CharT>
	void basic_fstream<CharT>::open(const string& filename, ios_base::openmode mode) {
		terminal_writestring("opening (string)...\n");
	}

	template<class CharT>
	void basic_fstream<CharT>::open(const char *filename, ios_base::openmode mode) {
		terminal_writestring("opening (char*)...\n");
	}

	// ctor
	template<class CharT>
	basic_fstream<CharT>::basic_fstream() { }
	// dtor
	template<class CharT>
	basic_fstream<CharT>::~basic_fstream() { }

	
}