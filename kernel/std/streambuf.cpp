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

	template<class CharT>
	void basic_streambuf<CharT>::setg(char_type* gbeg, char_type* gnext, char_type* gend)
	{
		g_beg = gbeg;
		g_cur = gnext;
		g_end = gend;
	}

	template<class CharT>
	void basic_streambuf<CharT>::setp(char_type* pbeg, char_type* pend)
	{
		p_beg = pbeg;
		p_end = pend;
	}

	// ctor
	template<class CharT>
	basic_streambuf<CharT>::basic_streambuf() { }
	// dtor
	template<class CharT>
	basic_streambuf<CharT>::~basic_streambuf() { }
}