// Shortcuter.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

// for Printf
#include <stdio.h>
#include <string>

// Include the Shortcuter Header File
#include "sx.hpp"
#include "config.hpp"


int main(int argc, char *argv[])
{
	// Create a new Config
	Config c;

	// Load the Shortcuts
	std::string content = loadConfig();
	// Parse KVP File
	auto result = KeyValueParser::parse_kvp2(content);

	c.NewStartMessage = checkNewStartMessage(result);

	// Check if Argumentsare here
	if (argc < 2)
	{
		if (!c.NewStartMessage)
		{
			printf("Please run with at least one argument or --help or -h.\n");
		}
		else
		{
			// printf("New Message should be added here\n");
			printNewStartMessage(result);
		}
		return 1;
	}

	// Print Help if requested
	if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")
	{
		return printhelp();
	}

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

	// Print Info
	if (std::string(argv[1]) == "--info" || std::string(argv[1]) == "-i")
	{
		return printinfo();
	}

	// Print Version
	if (std::string(argv[1]) == "--version" || std::string(argv[1]) == "-v")
	{
		std::cout << Version << "\n";
		return 0;
	}

	// Run Shortcut
	return runShortcut(argc, argv, result);;
}
