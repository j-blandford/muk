#include <std/fstream.hpp>


namespace std {
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

	// dtor
	template<class CharT>
	basic_filebuf<CharT>::~basic_filebuf() {
		if(is_open()) 
			close();
	}
}