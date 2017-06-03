#include <std/streambuf.hpp>

namespace std {
	/**********************************************
	* basic_streambuf<CharT>
	**********************************************/
	template class basic_streambuf<char>; // we need to provide a concrete implementation to link to

	template<class CharT>
	basic_streambuf<CharT>& basic_streambuf<CharT>::operator=(const basic_streambuf<CharT>& rhs)
	{
		return *this;
	}

	// ctor
	template<class CharT>
	basic_streambuf<CharT>::basic_streambuf() { }
	// dtor
	template<class CharT>
	basic_streambuf<CharT>::~basic_streambuf() { }
}