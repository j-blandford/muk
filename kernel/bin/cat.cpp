#include <std/fstream.hpp>

#include <kernel/bin/ls.hpp>
#include <kernel/tty.hpp>
#include <kernel/fs.hpp>
#include <kernel/fs/fat16.hpp>

int cat(std::vector<std::string> args) {
	std::fstream file;
	
	file.open("0:/home/james/login.profile", std::ios_base::in);

	std::string c_path = "/home/james/";
	std::string trim_path = c_path.substr(0,c_path.size()-1);

	// "home/james/login.profile"
	auto file_metadata = dynamic_cast<Filesystem::FAT16*>(Filesystem::devices[0])->walkSectors(0xC4);

	// "usr/conemu.xml"
	// auto file_metadata = dynamic_cast<Filesystem::FAT16*>(Filesystem::devices[0])->walkSectors(0xC5); 

	for(int sector : file_metadata) {
		terminal_printf(">>>> sec=%x\n", sector);

		uint8_t* sector_data = Filesystem::devices[0]->read_u8(27, sector * 0x200);

		if(sector_data != nullptr) 
			for(int i = 0; i < 26; i++) 
				terminal_printf("%c", (char)sector_data[i]);

		terminal_printf("\n");
	}

	return 0;
}