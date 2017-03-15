#pragma once

#include <stddef.h>
#include <stdint.h>

#include <kernel/fs.hpp>

fs_device_t * ramdisk_create(uintptr_t ptr, size_t capacity);
