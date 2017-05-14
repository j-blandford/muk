#include <kernel/bin/ls.hpp>
#include <kernel/tty.hpp>
#include <kernel/fs.hpp>
#include <kernel/fs/fat16.hpp>

int ls(std::vector<std::string> args) {
	std::string currentPath;

	// Is this an absolute or relative path? 
	// we need to add the current working directory if it's relative
	if(args[1][0] == '/') {
		// absolute path, do nothing to it :)
		currentPath = args[1];
	} 
	else {
		// Relative path
		currentPath = '/';
		currentPath += args[1];
		currentPath += '/';
	}

	for(auto dir : Filesystem::devices[0]->readDirectory(512)) {		
		terminal_printf("%s ", dir.name);

		if(strlen(dir.name) < 11) {
			terminal_writestring("\t");
		}

		terminal_printf("\t %s @ %x\n", dir.attributes == Filesystem::FATAttributes::shortNameFile ? "FILE" : "DIR", dir.location);
	}

	return 0;
}