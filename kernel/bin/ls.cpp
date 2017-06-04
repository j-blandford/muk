#include <kernel/bin/ls.hpp>
#include <kernel/tty.hpp>
#include <kernel/fs.hpp>
#include <kernel/fs/fat16.hpp>

int ls(std::vector<std::string> args) {
	std::string c_path = args[1];
	std::string trim_path = c_path.substr(0,c_path.size()-1);

	// append trailing slash
	if(trim_path[trim_path.size() - 1] != '/') {
		trim_path += '/';
	}

	for(auto dir : Filesystem::devices[0]->readDirectory((char*)trim_path)) {	
		bool is_dir = (dir.attributes != Filesystem::FATAttributes::shortNameFile);
		
		// metadata
		terminal_printf("%s- %x\t %d \tMay 14 17:04\t", is_dir ? "d" : "-", dir.location, dir.fsize);

		// name
		terminal_printf_rgba("%s\n", is_dir ? Graphics::RGB(0x3bff6e) : Graphics::RGB(0xFFFFFF), dir.name);
	}

	return 0;
}