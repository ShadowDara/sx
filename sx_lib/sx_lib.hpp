// SX Libary
//
// This is the header file for the SX Library,
// which provides functions for loading the Settings and
// other useful functions for the extensions for theSX Tool.
//


#pragma once

#include "KVP_single_header.hpp"
#include "ansicolors.hpp"
#include "../src/sx_oscore.hpp"

#include <string>
#include <vector>
#include <unordered_map>


// Function to load the Config File and return a Map with the Settings
std::unordered_map<std::string, std::string> loadConfig();


// Config for SX Addons and Extensions
struct SXLibConfig {
	// Prefix for the Settings
	std::string programName;

	std::string getSetting(const std::string& key,
		std::unordered_map<std::string, std::string> settings);

	SXLibConfig(const std::string& name) : programName(name) {}
};
