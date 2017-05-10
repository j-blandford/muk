#include <stddef.h>
#include <kernel/cpu.hpp>
#include <kernel/command.hpp>
#include <kernel/fs.hpp>

std::vector<std::string> Command::split(char* command) {
	std::vector<std::string> commandTokens;

	char* s = strtok(command, " ");

	// Lets split the command buffer
	while(s != NULL) {
		commandTokens.push_back(std::string(s));
		s = strtok(NULL, " ");
	}

	return commandTokens;
}

// void Command::Parse(char * buffer) {
// 	std::vector<string> tokens = Command::split(buffer);

// 	if(tokens[0] == "ls") {
// 		// ATA::getDirectoryPath(0, ENV::get("cd"));
// 		// update_buffer(false);
// 	} 
// 	else if(strncmp(tokens[0], "ps", 2) == 0) {
// 		ps(tokens);
// 	}

// 	else if(strncmp(tokens[0], "ls", 2) == 0) {
// 		ls(tokens);
// 	}

// 	else if(strncmp(tokens[0], "cd", 2) == 0) {
// 		if(tokens[1][0] == '/') {
// 			ENV::set("cd", tokens[1]);
// 		}
// 		else {
// 			string currentPath = ENV::get("cd");

// 			currentPath += tokens[1];
// 			currentPath += '/';
			
// 			ENV::set("cd", currentPath);
// 		}
// 	}
// }