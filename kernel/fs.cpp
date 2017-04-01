#include <kernel/fs.hpp>

namespace Filesystem {
	std::vector<FAT16 *> devices = std::vector<FAT16 *>();

	void initialise() {
		Filesystem::devices.push_back(new FAT16(0,0));
	}
}

