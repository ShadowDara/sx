#define KVP_IMPLEMENTATION
#include "KVP_single_header.hpp"

#include "sx_lib.hpp"


// Load the Config File and return a Map with the Settings
std::unordered_map<std::string, std::string> loadConfig()
{
	// Get Home Directory
	auto homeDir = getHomeDirectory();

	// Load KVP File
	std::ifstream file{ homeDir + "/sx.conf" };
	if (!file) {
		perror("Could not open Config File\n");
		return {};
	}

	std::string content(
		(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
	);

	auto result = KeyValueParser::parse_kvp2(content);

	return result;
}


// Load the Config File and return a Map with the Settings from
// the Argument given
std::unordered_map<std::string, std::string> loadConfigfromString(std::string arg)
{
	auto result = KeyValueParser::parse_kvp2(arg);

	return result;
}


// Load the Config and return the value for the given key
std::string SXLibConfig::getSetting(const std::string& key,
	std::unordered_map<std::string, std::string> settings)
{
	auto it = settings.find("--" + programName + "-" + key);
	if (it != settings.end())
	{
		return it->second;
	}
	return "";
}
