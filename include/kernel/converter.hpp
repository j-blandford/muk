#pragma once

#include <stddef.h>
#include <stdint.h>
#include <std.hpp>

namespace Converter {
	static const int kOneByte = sizeof(uint8_t);

	inline short To_16bit(uint8_t* start_byte) {
		return *start_byte
			| *(start_byte+kOneByte) << 8;
	}

	inline int To_32bit(uint8_t* start_byte) {
		return *start_byte
			| *(start_byte+kOneByte) << 8 
			| *(start_byte+kOneByte*2) << 16 
			| *(start_byte+kOneByte*3) << 24;
	}
};