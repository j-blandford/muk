#include <std/fstream.hpp>

#include <kernel/bin/ls.hpp>
#include <kernel/tty.hpp>
#include <kernel/fs.hpp>
#include <kernel/fs/fat16.hpp>

int cat(std::vector<std::string> args) {
	// std::fstream file;
	
	// file.open("0:/home/james/login.profile", std::ios_base::in);

	// // "home/james/login.profile"
	// //auto file_metadata = dynamic_cast<Filesystem::FAT16*>(Filesystem::devices[0])->walkSectors(0xC4);

	// // "usr/conemu.xml"
	// auto file_metadata = dynamic_cast<Filesystem::FAT16*>(Filesystem::devices[0])->walkSectors(0xC5); 


	if(args.size() > 1) {
		Filesystem::FAT16* device = dynamic_cast<Filesystem::FAT16*>(Filesystem::devices[0]);
		Filesystem::FileEntry file = device->readFile("/home/james/login.profile"); //args[1]);

		if(file.location != Filesystem::FileEntry::nfile) {
			bcprintf(">>>>>> Found %s\n", file.name);
			auto file_metadata = device->walkSectors(file.location); 

			for(int sector : file_metadata) {
				bcprintf("sec=%x\n", sector);

				uint8_t* sector_data = Filesystem::devices[0]->read_u8(27, sector * 0x200);

				if(sector_data != nullptr) 
					for(int i = 0; i < 512; i++) 
						terminal_printf("%c", (char)sector_data[i]);

			}
		}
	} 
	else {
		bcprintf("Error: no file specified.");
	}

	return 0;
}