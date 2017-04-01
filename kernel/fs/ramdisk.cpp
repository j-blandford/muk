#include <stddef.h>
#include <stdint.h>
#include <std.hpp>

#include <kernel/fs/ramdisk.hpp>

#include <kernel/memory/alloc.hpp>


// static bool ramdisk_read(fs_device_t* device, uint8_t *buffer, size_t offset, size_t size) {
//     memcpy(buffer, (void *)offset, size); // as ramdisk is stored in memory, it's very easy to grab the data
//     return true;
// }
// static bool ramdisk_write(fs_device_t* device, uint8_t *buffer, size_t offset, size_t size) {
//     // for the moment, ramdisks are read-only
//     return true;
// }

// // the following two functions are stubs just to fulfill the struct's contract
// static void ramdisk_open(fs_device_t* device) {
//     return;
// }
// static void ramdisk_close(fs_device_t* device) {
//     return;
// }

// fs_device_t * ramdisk_create(uintptr_t ptr, size_t capacity) {
//     fs_device_t * device = new fs_device_t("RAMDISK");

//     device->ptr = ptr;
//     device->capacity = capacity;

//     device->read = ramdisk_read;
//     device->write = ramdisk_write;
//     device->open = ramdisk_open;
//     device->close = ramdisk_close;

//     return device;
// }
