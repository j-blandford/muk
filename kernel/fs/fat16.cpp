#include <std/bitset.hpp>
#include <kernel/converter.hpp>
#include <kernel/fs/fat16.hpp>

#include <libcxxrt/typeinfo.h>

using namespace Filesystem;

std::vector<uint16_t> FileAllocationTable::walk(uint16_t start) {
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
bool FAT16::open() {
	uint8_t* temp_header = device->read_u8(1, 0);
	memcpy(&header_bytes, temp_header, 512);
	delete temp_header;

	bcprintf("Opening FS device...\n");

	// lets check to see if the device is FAT-formatted
	for(int i = 0; i < 3; i++) {
		if(*(header_bytes + i*sizeof(char)) != header[i]) 
			return false;
	}

	// the below variables scope out the "BIOS Parameter Block" (BPB)
	header_info.sectorSize = Converter::To_16bit(&header_bytes[11]);
	header_info.secPerCluster = header_bytes[13];
	header_info.numReservedSectors = Converter::To_16bit(&header_bytes[14]);
	header_info.numFATs = header_bytes[13];
	header_info.numDirectories = Converter::To_16bit(&header_bytes[17]);
	header_info.numSectors = Converter::To_16bit(&header_bytes[19]);
	header_info.numSectorsPerFAT = Converter::To_16bit(&header_bytes[21]);

	// Below we have the Extended BIOSParameter Block (EBPB)
	volume_serial = Converter::To_32bit(&header_bytes[39]);

	bcprintf(">>> Opened! Volume serial: %x\n", volume_serial);

	return true;
}

void FAT16::getFAT() {
	// Let us grab the allocation table
	fat = FileAllocationTable(header_info.numReservedSectors);

	uint8_t* temp_fat = this->device->read_u8(1, header_info.numReservedSectors);
	memcpy(&(fat.table), temp_fat, 512);
	delete temp_fat;
}

std::vector<DirectoryEntry> FAT16::readDirectory(unsigned int sectorIndex) {
	const std::bitset<32> bitMask(0x5542AA8A); // this is used to select out the long filename's data
	const size_t n_sectors = 1; // may be above 1 sector... look into this

	std::vector<DirectoryEntry> dir = std::vector<DirectoryEntry>();

	// Grab the dir table
	uint8_t* dir_bytes = this->device->read_u8(n_sectors, sectorIndex);

	size_t index = 0;
	while(index < n_sectors*ATA_BLOCKSIZE) {
		DirectoryEntry* dir_entry = new DirectoryEntry();

		dir_entry->attributes = static_cast<FATAttributes>(dir_bytes[index+11]);

		if(dir_entry->attributes == FATAttributes::shortName || dir_entry->attributes == FATAttributes::shortNameFile) {
			// Normal short name (8+3) structure
			char name[11] = {0};
			int length = 0;

			bool addSeperator = false; // adds a '.' char into the filename if 0x20 is found (might be bugged if the folder name has a space in it...)

			dir_entry->location = Converter::To_16bit(&(dir_bytes[index+26])); // 16-bit cluster location
			dir_entry->fsize = Converter::To_32bit(&(dir_bytes[index+28]));

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

			dir_entry->name[length] = '\0';
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

			dir_entry->name[length] = '\0';
			
			// now we grab some details which are located in the short-name entry...
			index += 32; 

			dir_entry->attributes = static_cast<FATAttributes>(dir_bytes[index+11]);
			dir_entry->location = Converter::To_16bit(&(dir_bytes[index+26])); // 16-bit cluster location
			dir_entry->fsize = Converter::To_32bit(&(dir_bytes[index+28]));
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

// uint16_t* FAT16::getChain(std::vector<uint16_t> chain) {
// 	// pass the return value of fat->walk() into this to grab the actual data
// 	uint16_t dataStart = 512 + ((header_info.numDirectories * 32) / header_info.sectorSize); // disk.img: this evaluates to 512+32 = 544
// 	uint16_t* chainBuffer = new uint16_t[chain.size()*header_info.sectorSize*header_info.secPerCluster];

// 	for(int i = 0; i < chain.size(); i++) {
// 		int sectorIndex = (chain[i] - 2) * header_info.secPerCluster + dataStart;

// 		this->device->read(&chainBuffer, 1, sectorIndex);
// 	}

// 	return chainBuffer;
// }

std::vector<int> FAT16::walkSectors(uint16_t startSector) {
	this->getFAT();

	std::vector<uint16_t> chain = this->fat.walk(startSector);
	std::vector<int> sectors = std::vector<int>();

	// pass the return value of fat->follow() into this to grab the actual data
	uint16_t dataStart = 512 + ((header_info.numDirectories * 32) / header_info.sectorSize); // disk.img: this evaluates to 512+32 = 544

	for(unsigned int i = 0; i < chain.size(); i++) {
		int sectorIndex = (chain[i] - 2) * header_info.secPerCluster + dataStart;

		sectors.push_back(sectorIndex);
	}
	
	return sectors;
}

std::vector<DirectoryEntry> FAT16::readDirectory(char* path) {
	std::vector<std::string> path_tokens = std::vector<std::string>();
	std::string str = path;
	size_t current_sec = 512; // 512 = root dir table sector

	if(str[0] == '/') {
		str = str.substr(1, str.size()-1);
	}

	// split "path" into tokens delimited by '/' and then extracts out each folder, 
	for (auto i = strtok(str.data(), "/"); i != nullptr; i = strtok(nullptr, "/")) {
		path_tokens.push_back(std::string(i));
	}

	// this code follows the structure (from the root node) to find the right table directory entry sector
	if(strlen(path) > 2) {
		for(std::string folder : path_tokens) {
			// We have to search the current sector for this particular folder token
			auto current_dir = this->readDirectory(current_sec);

			auto res = std::find_if(current_dir.begin(), 
							current_dir.end(), 
							[&folder] (DirectoryEntry dir) { return strcmp(dir.name, (char*)folder) == 0; });

			if(res != current_dir.end()) {
				// Folder was found! Set the current sector...
				current_sec = this->walkSectors((*res).location)[0];
			} 
			else {
				// Folder not found :(
				terminal_printf("Error: directory not found\n");
				
				return std::vector<DirectoryEntry>();
			}
		}
	}

	// "current_sec" now contains the found directory sector index
	return this->readDirectory(current_sec);
}

uint8_t* FAT16::read_u8(size_t numBytes, size_t offset) {
	// uint8_t* read_bytes = new uint8_t[num_bytes];
	// return read_bytes;

	return device->read_u8(1, offset / ATA_BLOCKSIZE);
}


FileEntry FAT16::readFile(char* path) {
	std::string p = path;
	std::string f = path;

	size_t slash = p.find_last_of("/")+1;

	p = p.substr(0, slash); // path element
	f = f.substr(slash);	// file element

	bcprintf("%s -- %s\n", (char*)p, (char*)f);

	for(DirectoryEntry entry : this->readDirectory((char*)p)) {
		if(std::string(entry.name) == (char*)f) {
			FileEntry fe;
			fe.size = entry.fsize;
			fe.location = entry.location;
			fe.name = new char[strlen(entry.name)];

			memcpy(fe.name, entry.name, strlen(entry.name));

			return fe;
		}
	}

	return FileEntry();
}