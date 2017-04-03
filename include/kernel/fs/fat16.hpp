#pragma once

#include <kernel/fs.hpp>
#include <kernel/drivers/ata.hpp>
#include <kernel/memory/alloc.hpp>

namespace Filesystem {
	// this is essentially a wrapper for linked list functionality in the allocation table
	struct FileAllocationTable {
		int tableOffset;
		uint8_t table[512]; 

		FileAllocationTable(uint8_t* table_ptr, int offset) : tableOffset(offset){ memcpy(table, table_ptr, 512); }
		FileAllocationTable(int offset) : tableOffset(offset) { }
		FileAllocationTable() { }

		std::vector<uint16_t> walk(uint16_t start); // returns a vector of 16-bit integers (the clusters in the chain)

		void printFAT();
	};


	struct FATHeader {
		unsigned short sectorSize;     // sector size, usually 512
		int secPerCluster;      // how many sectors make up a cluster (usually 2)

		int numReservedSectors; // number of pre-data reserved sectors (usually 2)

		int numFATs;            // number of file allocation tables (usually 2)
		uint16_t numSectorsPerFAT; // number of sectors in each file allocation table

		uint16_t numDirectories;
		uint32_t numSectors;    // total number of sectors in the volume.
	};

	class FAT16 : public IDevice {
		const uint8_t header[3] = {0xEB, 0x3C, 0x90}; // FAT16 header bytes
		ATA* device;

		uint8_t header_bytes[512]; 
		uint8_t fat_table[512]; 
		FATHeader header_info;
		FileAllocationTable fat;
	public:
		uint32_t volume_serial;
		
		FAT16(int bus, int drive) { 
			device = new ATA(bus, drive);

			if(!open()) {
				terminal_writestring("[FAT16] Device not recognised.\n");
			} 
			else {
				terminal_writestring("[FAT16] Verified FAT header.\n");
				this->getFAT();
			}
		}
		~FAT16() { }

		bool open();
		void read(uint16_t** buffer, size_t numBytes, size_t offset);
		void getFAT();
		std::vector<int> walkSectors(uint16_t startSector);
		std::vector<DirectoryEntry> readDirectory(unsigned int sectorIndex);
		std::vector<DirectoryEntry> readDirectory(char* path);
	};



	
}