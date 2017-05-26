#pragma once
#include <stddef.h>

#include <std.hpp>
#include <std/algorithm.hpp>
#include <kernel/cpu.hpp>

int strlen( char* ptr );
int strcmp( const char* s1, const char* s2 );
void strncpy( char* ptr_dest, char* ptr_src, int n );

namespace std {
	class string {
		using size_type = std::size_t;

		size_type length;               // length of the string
		char* buff;              // pointer to strorage
	public:
		static const size_type npos = -1; // maximum size_t value
		
		// Constructors
		string() : length(0), buff(NULL) { }
		string(char* init_val) : length(strlen(init_val)), buff(new char[length]) {
			strncpy( buff, init_val, length );  // copy init value into storage
		}
		string(char init_val) : length(1), buff(new char[1]) {
			buff[0] = init_val;
		}
		string(const string& other) : length(other.length), buff(new char[length]) {
			strncpy( buff, other.buff, length );
		}
		~string() {
			delete [] buff;
		}

		size_type size() const { return length; }
		char* c_str() { return buff; }

		const char* data() const { return buff; }
		const char data(size_type index) const { return buff[index]; }

		char* data() { return buff; }
		char data(size_type index) { return buff[index]; }

		int compare( size_type pos1, size_type count1, const string& str ) const {
			int result = 0;

			if(pos1+count1 > this->size() || count1 == std::string::npos) 
				count1 = this->size() - pos1;

			for(size_type p = 0; p < std::min<size_type>(count1, str.size()); p++) {
				if((int)this->data(p + pos1) > (int)str.data(p)) {
					++result;
					break;
				} 
				else if((int)this->data(p + pos1) < (int)str.data(p)) {
					--result;
					break;
				}
			}

			return result;
		}

		size_type find( const string& str, size_type pos = 0 ) const {
			for(size_type i = pos; i < this->size(); ++i) {
				if(this->compare(i, std::string::npos, str) == 0) {
					return i;
				}
			}

			return std::string::npos;
		}

		string substr(size_type pos = 0, size_type count = std::string::npos) const {
			if(pos + count > this->size() || count == std::string::npos) 
				count = this->size() - pos;
			
			char* dest = new char[count+1];
			
			for(size_type p = 0; p < count; p++) {
				dest[p] = this->data(pos + p);
			}

			dest[count] = '\0';
			
			std::string str = dest;
			delete dest;

			return str;
		}

		// size_type find( const char* s, size_type pos = 0 ) const {
		// 	string comp = string(s);
		// 	for(size_type i = pos; i < this->size(); ++i) {
		// 		if(this->compare(i, std::string::npos, comp) == 0) {
		// 			return i;
		// 		}
		// 	}
		// }

		// Operator overloads
		string& operator= ( const string& other ) {
			if( this != &other ){          // guard against  a = a;
				this->length = other.length;       // allocate new memory
				this->buff = new char[length];

				strncpy( this->buff, (char*)other.data(), this->length );
			}

			return *this;
		}

		string& operator= ( char* other ) {
			length = strlen(other);       // allocate new memory
			buff = new char[length];
			strncpy( buff, other, length );

			return *this;
		}


		char& operator[] ( size_type index ) {
			return buff[index];
		}

		char operator[] ( size_type index ) const {
			return buff[index];
		}

		friend string operator+( const string& s1, const string& s2 );
		friend string operator+( const string& s, char c );
		friend string operator+( char c, const string& s );

		bool operator== (const string& rhs) {
			return strcmp(buff, rhs.buff) == 0;
		}
		bool operator== (const char* rhs) {
			return strcmp(buff, rhs) == 0;
		}

		string& operator+=( const string& s2 ) {
			char* newBuff = new char[this->length+s2.length];

			strncpy( newBuff, this->buff, this->length);
			strncpy( newBuff + this->length, s2.buff, s2.length);

			this->buff = newBuff;
			this->length += s2.length;

			return *this;
		}

		string& operator+=( const char& s2 ) {
			char* newBuff = new char[this->length+1];
			strncpy( newBuff, buff, this->length);

			newBuff[this->length] = s2;

			this->buff = newBuff;
			this->length += 1;

			return *this;
		}

		// string& operator+=( const char* s2 ) {
		// 	char* newBuff = new char[this->length+strlen((char*)s2)+1];
		// 	strncpy( newBuff, buff, this->length);
		// 	strncpy( *(&newBuff+this->length), (char*)s2, strlen((char*)s2));

		// 	this->buff = newBuff;
		// 	this->length = strlen(this->buff);

		// 	return *this;
		// }

		operator char*() const {
			char * nullBuffer = new char[this->length+1];

			strncpy( nullBuffer, this->buff, this->length);
			nullBuffer[this->length] = '\0';

			return nullBuffer;
		}

	};
}