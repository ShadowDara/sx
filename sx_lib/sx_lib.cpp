#define KVP_IMPLEMENTATION
#include "KVP_single_header.hpp"

#include "sx_lib.hpp"


void SXLibConfig::loadConfig()
{
	// Get Home Directory
	auto homeDir = getHomeDirectory();

	// Load KVP File
	std::ifstream file{ homeDir + "/sx.conf" };
	if (!file) {
		perror("Could not open Config File\n");
		return "";
	}

	std::string content(
		(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
	);

	auto result = KeyValueParser::parse_kvp2(content);

	settings = std::move(result);
}


std::string SXLibConfig::getSetting(const std::string& key) const;
{
	auto it = settings.find("--" + programName + "-" + key);
	if (it != settings.end())
	{
		return it->second;
	}
	return "";
}
