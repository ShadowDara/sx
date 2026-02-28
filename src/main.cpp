// Shortcuter.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include <stdio.h>
#include "oscore.hpp"
#include <string>

#include "sc.hpp"


int main(int argc, char *argv[])
{
	// Check Arguments here
	if (argc < 2) {
		printf("Please run with at least one argument or --help or -h.\n");
		return 1;
	}

	// Print Help if requested
	if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
		return printhelp();
	}

	// Load the Config
	std::string content = loadConfig();
	// Parse KVP File
	auto result = KeyValueParser::parse_kvp2(content);

	// Print View
	// Iterate over the result and print the key-value pairs
	if (std::string(argv[1]) == "--view" || std::string(argv[1]) == "-v")
	{
		// Iterieren und ausgeben
		for (const auto& pair : result) {
			std::cout << pair.first << " => " << pair.second << '\n';
		}

		return 0;
	}

	// Run Shortcut
	return runShortcut(argc, argv, result);;
}
