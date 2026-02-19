// Shortcuter.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include <stdio.h>
#include <kvp.hpp>
#include "oscore.hpp"
#include <fstream>


int main(int argc, char *argv[])
{
	// Check Arguments here
	if (argc < 2) {
		printf("Please run with at least one argument or --help.\n");
		return 1;
	}

	// Print Help if requested
	if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
		printf(
			"Usage: shortcuter [Command Shortcut] {Arguments ...}\n"
			"\n"
			"Example Configuration File:\n"
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
			"\n"
			"hi=echo Hallo World\n"
		);
		return 0;
	}

	// Get Home Directory
	auto homeDir = getHomeDirectory();

	// Load KVP File
	std::ifstream file{ homeDir + "/sx.conf"};
	if (!file) {
		perror("Could not open Config File\n");
		return 1;
	}

	std::string content(
		(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
	);

	// Parse KVP File
	auto result = KeyValueParser::parse_kvp2(content);

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
