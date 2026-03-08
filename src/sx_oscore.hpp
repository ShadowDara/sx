#pragma once

#include <cstdlib>
#include <string>


// Function to get the user's home directory
// in a cross-platform way
std::string getHomeDirectory();

// Function to run a command in the system shell
int runCommand(
	const std::string& command,
	const std::string& terminal,
	const std::string& linuxterminal
);
