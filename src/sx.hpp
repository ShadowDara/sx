// Shortcuter Header File in C++

#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>

#include "sx_config.hpp"
#include "sx_oscore.hpp"

#include "../sx_lib/ansicolors.hpp"
#include "../sx_lib/KVP_single_header.hpp"


// Function to print the help message
int printhelp();

// Function to print an Info Message
int printinfo();

// Function to load the configuration file and
// return its content as a string
std::string loadConfig();

// Function to run the shortcut command
int runShortcut(int argc, char* argv[],
	std::unordered_map<std::string, std::string>& result
);

// Function to check if the Defaul Message with no arguments go
// Overwritten
bool checkNewStartMessage(std::unordered_map<std::string, std::string>& result);

// Function to print the new Start Message
// while be first searched for the file path from the hash map and then
// the File path will be loaded, and file content will be written to the
// Terminal
void printNewStartMessage(std::unordered_map<std::string, std::string>& result);

// // Function to change a Command
// int changeCommand(int argc, char* argv[]);
