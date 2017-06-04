#pragma once

#include <stddef.h>
#include <stdint.h>
#include <std.hpp>
#include <std/vector.hpp>

#include <kernel/memory/alloc.hpp>

namespace Filesystem {
	enum FATAttributes : uint8_t {
		noEntry = 0x00,
		shortNameFile = 0x20,
		shortName = 0x10,
		longName = 0x0F
	};

	struct PathElements {
		std::string drive;
		std::vector<std::string> tokens;
		
		PathElements()
		: tokens(std::vector<std::string>()) 
		{ }
	};

    class DirectoryEntry {
	public:
		char* name;
		char* altName;     // used in FAT16 for the long-hand name

		FATAttributes attributes;

		// TODO: Create a Timestamp class
		short mod_time;      // modification time (H:m:s)
		short mod_date;      // modification date
		short crt_time;      // creation time (H:m:s)
		short crt_date;      // creation date

		short location;     // generic "cluster" location
		int fsize;     // generic "cluster" location

		DirectoryEntry() 
		: name(new char[32])
		, location(0) 
		, fsize(0)
		{ 
			memset(name, 0, 11); 
		}

		DirectoryEntry(char* name) 
		: location(0)
		, fsize(0) { }

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
        virtual uint8_t* read_u8(size_t numBytes, size_t offset) = 0;
		virtual std::vector<DirectoryEntry> readDirectory(unsigned int sectorIndex) = 0;
        virtual std::vector<DirectoryEntry> readDirectory(char* path) = 0;
    };

	extern std::vector<IDevice*> devices;

	void initialise();
	PathElements splitPath(std::string path);
}


