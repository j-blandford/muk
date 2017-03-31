#pragma once

#include <stddef.h>
#include <stdint.h>

#include <std.hpp>

#include <kernel/tty.hpp>
#include <kernel/drivers/ata.hpp>

typedef struct fat_header {
	unsigned short sectorSize;     // sector size, usually 512
	int secPerCluster;      // how many sectors make up a cluster (usually 2)

	int numReservedSectors; // number of pre-data reserved sectors (usually 2)

	int numFATs;            // number of file allocation tables (usually 2)
	uint16_t numSectorsPerFAT; // number of sectors in each file allocation table

	uint16_t numDirectories;
	uint32_t numSectors;    // total number of sectors in the volume.
}__attribute__((packed)) fat_header_t;

class FAT16 {
	const uint8_t header[3] = {0xEB, 0x3C, 0x90}; // FAT16 header bytes
	ATA* device;

	uint8_t header_bytes[512]; 
	uint8_t fat_table[512]; 
	fat_header header_info;
public:
	uint32_t volume_serial;
	
	FAT16(int bus, int drive) { 
		device = new ATA(bus, drive);

		if(!initialiseDevice()) {
			terminal_writestring("[FAT16] Device not recognised.");
		} else {
			terminal_writestring("[FAT16] Verified FAT header.");
		}
	}
	~FAT16() { }

	bool initialiseDevice();
};