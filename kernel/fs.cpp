#include <kernel/fs.hpp>
#include <kernel/fs/fat16.hpp>

namespace Filesystem {
	std::vector<IDevice*> devices = std::vector<IDevice*>();

	void initialise() {
		devices.push_back(new FAT16(0,0));
	}
}