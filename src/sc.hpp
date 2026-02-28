// Shortcuter Header File in C++

#pragma once

#include <stdio.h>
#include <string>
#include <fstream>
#include <unordered_map>

#include "oscore.hpp"

#include "../KVP_single_header.hpp"

// Function to print the help message
int printhelp();

// Function to load the configuration file and
// return its content as a string
std::string loadConfig();

// Function to run the shortcut command
int runShortcut(int argc, char* argv[],
	std::unordered_map<std::string, std::string> result
);
