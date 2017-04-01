#include <std/bitset.hpp>
#include <kernel/fs/fat16.hpp>

void Filesystem::FileAllocationTable::printFAT() {
	// for(int i = 0; i < 512; i++) {
	// 	terminal_printf("%x ", table[i]);
	// }	
}

std::vector<uint16_t> Filesystem::FileAllocationTable::walk(uint16_t start) {
	// "start" is the value grabbed from the directory tables!!!
	std::vector<uint16_t> *chain = new std::vector<uint16_t>();

	uint16_t current = start;
	while(current < 0xF8) {
		(*chain).push_back(current);
		current = table[current*2]; // | table[current*2+1] >> 8;
	}

	return *chain;
}

// this function gets called in the object constructor and verifies
// that the device's header is actually FAT16-compliant.
bool Filesystem::FAT16::open() {
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

void Filesystem::FAT16::read(uint16_t** buffer, size_t numBytes, size_t offset) {

}

void Filesystem::FAT16::getFAT() {
	// Let us grab the allocation table
	fat = FileAllocationTable(header_info.numReservedSectors);

	uint8_t* temp_fat = new uint8_t[512];
	this->device->read(&temp_fat, 1, header_info.numReservedSectors);
	memcpy(&(fat.table), temp_fat, 512);
}

std::vector<Filesystem::DirectoryEntry> Filesystem::FAT16::readDirectory(unsigned int sectorIndex) {
	const std::bitset<32> bitMask(0x5542AA8A); // this is used to select out the long filename's data
	const size_t n_sectors = 1; // may be above 1 sector... look into this

	this->getFAT();

	std::vector<Filesystem::DirectoryEntry> dir = std::vector<Filesystem::DirectoryEntry>();
	uint8_t* dir_bytes = new uint8_t[512]; 

	// Grab the dir table
	this->device->read(&dir_bytes, n_sectors, sectorIndex);

	// for(int i = 0; i < 32; i++) {
	// 	terminal_printf("%x ", dir_bytes[i]);
	// }	

	size_t index = 0;
	while(index < n_sectors*ATA_BLOCKSIZE) {
		DirectoryEntry* dir_entry = new DirectoryEntry();

		dir_entry->attributes = dir_bytes[index+11];

		if(dir_entry->attributes == FATAttributes::shortName || dir_entry->attributes == FATAttributes::shortNameFile) {
			// Normal short name (8+3) structure
			char name[11] = {0};
			int length = 0;

			bool addSeperator = false; // adds a '.' char into the filename if 0x20 is found (might be bugged if the folder name has a space in it...)

			dir_entry->location = dir_bytes[index+26] | (dir_bytes[index+27] << 8); // 16-bit cluster location

			// the first 11 bytes store the full short foldername. Let us extract it out...
			for(size_t offset = 0; offset <= 11; offset++) {
				char dirChar = dir_bytes[index+offset];

				if(dirChar != 0x20 && dirChar != 0x10) { // 0x20 and 0x10 are padding or "file+ext" seperator byte
					if(addSeperator) {
						dir_entry->name[length] = '.';
						length++;

						addSeperator = false;
					}

					dir_entry->name[length] = tolower(dirChar);
					length++;
				} 
				else {
					addSeperator = true;
				}
			}
		} 
		else if(dir_entry->attributes == FATAttributes::longName) {
			// We have found a LONG FOLDERNAME
			char name[32];
			int length = 0;

			// Our foldername is embedded inside 32-bytes. We have to extract it out the long way
			for(size_t j = 0; j <= 32; j++) {
				// our bitmask contains a series of 0 and 1s which allow us to extract the name correctly
				if(bitMask.test(31-j) && dir_bytes[index+j] != 0xFF && dir_bytes[index+j] != 0x00) {
					dir_entry->name[length] = dir_bytes[index+j];
					length++;
				}
			}
			
			// now we grab some details which are located in the short-name entry...
			index += 32; 

			dir_entry->attributes = dir_bytes[index+11];
		}
		else if(dir_entry->attributes == FATAttributes::noEntry) {
			// there isn't a file here, so just skip to the next index entry
			index += 32;
			continue;
		}

		dir.push_back(*dir_entry);

		index += 32;
	}

	return dir;
}