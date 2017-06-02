#include <kernel/fs.hpp>
#include <kernel/fs/fat16.hpp>

namespace Filesystem {
	std::vector<IDevice*> devices = std::vector<IDevice*>();

	void initialise() {
		devices.push_back(new FAT16(0,0));
	}

	PathElements splitPath(std::string path) {
		PathElements p_elements;

		if(path.find(":") != path.npos) {
			// we have a drive prepending the path, let's extract it out
			p_elements.drive = "0";
		}
		else {
			p_elements.drive = "0"; // default to the boot drive
		}

		return p_elements;
	}
}