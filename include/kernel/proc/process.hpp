#pragma once

#include <kernel/memory/physical.hpp>

class Process {
public:
	size_t proc_id;
	char* title;

	page_directory_t pd;
};