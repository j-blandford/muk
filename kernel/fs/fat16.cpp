#include <stddef.h>
#include <std.hpp>
#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/timer.hpp>
#include <kernel/drivers/ata.hpp>
#include <kernel/fs/fat16.hpp>
#include <kernel/memory/alloc.hpp>


// this function gets called in the object constructor and verifies
// that the device's header is actually FAT16-compliant.
bool FAT16::initialiseDevice() {
	uint8_t* temp_header = new uint8_t[512];

	device->read(&temp_header, 1, 0);
	memcpy(&header_bytes, temp_header, 512);

	for(int i = 0; i < 3; i++) {
		if(*(header_bytes + i*sizeof(char)) != header[i]) return false;
	}

	// the below variables scope out the "BIOS Parameter Block" (BPB)
	// TODO: make a struct that follows the byte pattern of the header so we can hydrate the class in with malloc+memset
	header_info.sectorSize = (*(header_bytes + 11*sizeof(char))) | (*(header_bytes + 12*sizeof(char)) << 8); // little-endian -> big-endian
	header_info.secPerCluster = *(header_bytes + 13*sizeof(char));
	header_info.numReservedSectors = *(header_bytes + 14*sizeof(char)); // offset 15 would be zero in like 99.9999% of cases
	header_info.numFATs = *(header_bytes + 16*sizeof(char));
	header_info.numDirectories = (*(header_bytes + 17*sizeof(char))) | (*(header_bytes + 18*sizeof(char)) << 8);
	header_info.numSectors = (*(header_bytes + 19*sizeof(char))) | (*(header_bytes + 20*sizeof(char)) << 8);
	header_info.numSectorsPerFAT = (*(header_bytes + 21*sizeof(char))) | (*(header_bytes + 22*sizeof(char)) << 8);

	// Below we have the Extended BIOSParameter Block (EBPB)
	volume_serial = (*(header_bytes + 39*sizeof(char))) 
					| (*(header_bytes + 40*sizeof(char)) << 8) 
					| (*(header_bytes + 41*sizeof(char)) << 16) 
					| (*(header_bytes + 42*sizeof(char)) << 24);

	terminal_printf("\tVolume serial: %x\n", volume_serial);

	return true;
}