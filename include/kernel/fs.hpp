#pragma once

#include <stddef.h>
#include <stdint.h>
#include <std.hpp>
#include <std/vector.hpp>

#include <kernel/fs/fat16.hpp>
#include <kernel/memory/alloc.hpp>

namespace Filesystem {
	extern std::vector<FAT16 *> devices;

	void initialise();
}


// VVVVVVVVVVVV NEED TO INTEGRATE THIS C CODE VVVVVVVVVVVVVVVVVV

// This is a generic interface for all filesystem devices. To add your own device, just fulfill the 4 function pointers and you're set!
struct fs_device_t;

// our function pointer definitions
using fs_read_t = bool (fs_device_t*, uint8_t *, size_t, size_t); // device, buffer, offset, length
using fs_write_t = bool (fs_device_t*, uint8_t *, size_t, size_t);
using fs_open_t = void (fs_device_t*);
using fs_close_t = void (fs_device_t*);

struct fs_device_t {
    char* name;
    uintptr_t ptr;
    size_t capacity;

    fs_read_t* read;
    fs_write_t* write;
    fs_open_t* open;
    fs_close_t* close;

    fs_device_t(char* devName) : name(new char[255]) { memcpy(name, devName, 8); } //strlen(devName)

//    fs_readdir_t readdir; // SOON :)
};
