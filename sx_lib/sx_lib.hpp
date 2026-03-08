#pragma once

#include "KVP_single_header.hpp"
#include "ansicolors.hpp"

#include <string>
#include <vector>
#include <unordered_map>


struct SXLibConfig {
	// Prefix for the Settings
	std::string programName;

	std::unordered_map<std::string, std::string> settings;

	void loadConfig();

	std::string getSetting(const std::string& key) const;
};
