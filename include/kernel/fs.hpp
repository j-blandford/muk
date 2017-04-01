#pragma once

#include <stddef.h>
#include <stdint.h>
#include <std.hpp>
#include <std/vector.hpp>

#include <kernel/memory/alloc.hpp>

namespace Filesystem {
    
    class DirectoryEntry {
	public:
		char* name;
		char* altName;     // used in FAT16 for the long-hand name

		short attributes;

		// TODO: Create a Timestamp class
		short mod_time;      // modification time (H:m:s)
		short mod_date;      // modification date
		short crt_time;      // creation time (H:m:s)
		short crt_date;      // creation date

		short location;     // generic "cluster" location

		DirectoryEntry() : name(new char[50]), location(0)  { }
		DirectoryEntry(char* name) : location(0) { }
		~DirectoryEntry() {
			delete name;
			delete altName;
		}
	};

    // This is a generic interface for all filesystem devices. To add your own device, 
    // just fulfill the 4 function pointers and you're set!
    class IDevice {
    public:
        virtual bool open() = 0;
        virtual void read(uint16_t** buffer, size_t numBytes, size_t offset) = 0;
        virtual std::vector<DirectoryEntry> readDirectory(unsigned int sectorIndex) = 0;
    };

	extern std::vector<IDevice*> devices;

	void initialise();
}


