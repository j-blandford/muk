#include <std/fstream.hpp>

#include <kernel/bin/ls.hpp>
#include <kernel/tty.hpp>
#include <kernel/fs.hpp>
#include <kernel/fs/fat16.hpp>

int cat(std::vector<std::string> args) {
	std::fstream file;
	
	file.open("0:/usr/login.profile", std::ios_base::in);

	// for(auto dir : Filesystem::devices[0]->readDirectory((char*)trim_path)) {	
	// 	bool is_dir = (dir.attributes != Filesystem::FATAttributes::shortNameFile);
		
	// 	// metadata
	// 	terminal_printf("%s- %x\t May 14 17:04\t", is_dir ? "d" : "-", dir.location);

	// 	// name
	// 	terminal_printf_rgba("%s\n", is_dir ? Graphics::RGB(0x3bff6e) : Graphics::RGB(0xFFFFFF), dir.name);
	// }

	return 0;
}