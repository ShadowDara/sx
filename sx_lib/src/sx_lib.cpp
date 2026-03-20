#include <sx_lib/sx_lib.hpp>


// Load the Config File and return a Map with the Settings
std::unordered_map<std::string, std::string> loadConfig()
{
	// Get Home Directory
	auto homeDir = getHomeDirectory();

	// Load KVP File
	std::ifstream file{ homeDir + "/sx.conf" };
	if (!file) {
		std::cerr << RED "Could not open Config File" END << std::endl;
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


// Function to check a Boolean Entry from the Config
bool checkBoolEntry(const std::unordered_map<std::string, std::string>& result,
	const std::string& name)
{
	// Check for Standard print overwriting
	auto it = result.find(name);
	if (it == result.end())
	{
		return false;
	}

	// Set in the Config to true
	std::string OverrwriteStartMessage = it->second;
	if (OverrwriteStartMessage == "true")
	{
		return true;
	}

	return false;
}
