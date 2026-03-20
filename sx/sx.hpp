// Shortcuter Header File in C++

#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>

#include "sx_config.hpp"
#include "../sx_lib/sx_lib_oscore.hpp"

#include "../sx_lib/ansicolors.hpp"


// Function to print the help message
int printhelp();

// Function to print an Info Message
int printinfo();

// Function to run the shortcut command
int runShortcut(int argc, char* argv[],
	std::unordered_map<std::string, std::string>& result
);

// Function to print the new Start Message
// while be first searched for the file path from the hash map and then
// the File path will be loaded, and file content will be written to the
// Terminal
void printNewStartMessage(std::unordered_map<std::string, std::string>& result);

// // Function to change a Command
// int changeCommand(int argc, char* argv[]);
