#define KVP_IMPLEMENTATION
#include "../KVP_single_header.hpp"

#include "sc.hpp"

// Function to print the help message
int printhelp()
{
	printf(
		"Usage: shortcuter [Command Shortcut] {Arguments ...}\n"
		"\n"
		"Example Configuration File:\n"
		"\n"
		"# Config File for shortcuter\n"
		"# \n"
		"# Lines which start with #, are comments!\n"
		"# \n"
		"# Add KeyValuePairs here not for command shortcuts\n"
		"# The left is the shortcut and the right is the long command\n"
		"# which will be executed!\n"
		"#\n"
		"# All Arguments after the the shortcut command will be added\n"
		"# to the runned command!\n"
		"#\n"
		"hi=echo Hallo World\n"
		"\n"
		"--view, -v\tView all shortcuts\n"
	);
	return 0;
}


// Function to load the configuration file and
// return its content as a string
std::string loadConfig()
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

	return content;
}


// Function to run the shortcut command
int runShortcut(int argc, char* argv[],
	std::unordered_map<std::string, std::string> result)
{
	// Check if shortcut exists
	auto it = result.find(argv[1]);
	if (it == result.end()) {
		printf("Shortcut not found: %s\n", argv[1]);
		return 1;
	}

	// Base command from config
	std::string command = it->second;


	// Append additional arguments
	for (int i = 2; i < argc; ++i) {
		command += " ";
		command += argv[i];
	}

	// Execute
	auto returncode = runCommand(command);

	//printf("Command returned with code %d\n", returncode);

	return returncode;
}
