#pragma once

#include <stddef.h>
#include <std.hpp>

#include <kernel/user/env.hpp>

namespace Ext {
	using Function = int (*) (std::vector<std::string>);

	class CmdPair {
		std::string cmd;
		Function ptr;
	public:
		CmdPair() { }
		CmdPair(std::string cmd, Function ptr) 
		: cmd(cmd)
		, ptr(ptr) { }
		std::string getCmd() { return cmd; }
		Function getPtr() { return *ptr; }
	};

	class Library {
		std::vector<CmdPair*> commands;
	public:
		Library() {
			commands = std::vector<CmdPair*>();
		}
		
		Function operator[] ( char* idx );
		void add(char* cmd, Function ptr);
		bool present(char* cmd);
		Function get(char* cmd);
	};

	extern Library lib;

	void load();
};