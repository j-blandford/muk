#include <stddef.h>

#include <kernel/cpu.hpp>
#include <kernel/ext.hpp>

#include <kernel/bin/ps.hpp>
#include <kernel/bin/ls.hpp>
#include <kernel/bin/cat.hpp>

namespace Ext {
	Library lib;

	void Library::add(char* cmd, Function ptr) {
		commands.push_back(new CmdPair(std::string(cmd), ptr));
	}

	bool Library::present(char* cmd) {
		for(auto it : commands) {
			if(strcmp((char*)it->getCmd(), cmd) == 0) 
				return true;
		}

		return false;
	}

	Function Library::operator[] ( char* cmd ) {
		for(auto it : commands) {
			if((char*)it->getCmd() == cmd)
				return it->getPtr();
		}
		// should never reach here...
		return nullptr;
	}


	Function Library::get( char* cmd ) {
		for(auto it : commands) {
			if(strcmp((char*)it->getCmd(), cmd) == 0)
				return it->getPtr();
		}
		// should never reach here...
		return nullptr;
	}

	void load() {
		lib.add("ps", &ps);
		lib.add("ls", &ls);
		lib.add("cat", &cat);
	}
}