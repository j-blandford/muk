#pragma once

#include <stddef.h>
#include <stdint.h>

// This is a generic interface for all filesystem devices. To add your own device, just fulfill the 4 function pointers and you're set!
struct fs_device_t;

// our function pointer definitions
using fs_read_t = bool(*)       (fs_device_t, uint8_t *, size_t, size_t); // device, buffer, offset, length
using fs_write_t = bool(*)      (fs_device_t, uint8_t *, size_t, size_t);
using fs_open_t = void(*)   (fs_device_t);
using fs_close_t = void(*)  (fs_device_t);

struct fs_device_t {
    char* name[255];
    uintptr_t ptr;
    size_t capacity;

    fs_read_t read;
    fs_write_t write;
    fs_open_t open;
    fs_close_t close;

//    fs_readdir_t readdir; // SOON :)
};
