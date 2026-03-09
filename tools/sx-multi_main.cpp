// Main file for the Multi Tool

#include <iostream>

#include "../src/sx-multi_config.hpp"
#include "../src/sx-multi.hpp"
#include "../sx_lib/sx_lib.hpp"


// Main function for the Multi Tool
int main(int argc, char* argv[])
{
	// Config
	auto config = loadConfig();

	// Number Guess Config
	auto numberguessConfig = SXLibConfig("numberguess");
	
	// Check if Argumentsare here
	if (argc < 2)
	{
		std::cout << "Please run with at least one argument or --help or -h.\n";
		return 1;
	}

	// Print Help if requested
	if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")
	{
		return printhelp();
	}

	// Check if the numberguess game should be started
	if (std::string(argv[1]) == "--numberguess" || std::string(argv[1]) == "-n")
	{
		//std::cout << numberguessConfig.getSetting("lol", config);
		numberguess();
		return 0;
	}
}
