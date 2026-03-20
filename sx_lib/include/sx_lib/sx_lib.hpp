// SX Libary
//
// This is the header file for the SX Library,
// which provides functions for loading the Settings and
// other useful functions for the extensions for theSX Tool.
//


#pragma once

#include <kvp/kvp.hpp>

#include "ansicolors.hpp"
#include "sx_lib_oscore.hpp"

#include <string>
#include <vector>
#include <unordered_map>


// Function to load the Config File and return a Map with the Settings
std::unordered_map<std::string, std::string> loadConfig();

// Load the Config File and return a Map with the Settings from
// the Argument given
std::unordered_map<std::string, std::string> loadConfigfromString(std::string arg);


// Config for SX Addons and Extensions
struct SXLibConfig {
	// Prefix for the Settings
	std::string programName;

	std::string getSetting(const std::string& key,
		std::unordered_map<std::string, std::string> settings);

	SXLibConfig(const std::string& name) : programName(name) {}
};


// Function to check a Boolean Entry from the Config
bool checkBoolEntry(const std::unordered_map<std::string, std::string>& result,
	const std::string& name);
