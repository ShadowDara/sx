// Main file for the Multi Tool

#include <iostream>
#include <charconv>

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
		
		std::string minS = numberguessConfig.getSetting("min", config);
		std::string maxS = numberguessConfig.getSetting("max", config);
		
		int min;
		int max;
		
		auto result = std::from_chars(minS.data(), minS.data() + minS.size(), min);
		auto result2 = std::from_chars(maxS.data(), maxS.data() + maxS.size(), max);
		
		if (result.ec == std::errc() && result2.ec == std::errc())
		{
			numberguess(min, max);
		}
		else
		{
			numberguess(1, 1000);
		}

		return 0;
	}

	// Check for the loadtime function
	if (std::string(argv[1]) == "--loadtime" || std::string(argv[1]) == "-l")
	{
		std::string content = "";
		bool useOtherConfig = true;

		if (argc > 2)
		{
			std::string filename = argv[2];

			std::cout << "Trying to load file: " << filename << std::endl;

			std::ifstream file{ filename };
			if (!file) {
				perror("Could not open Config File\n");
				useOtherConfig = false;
			}

			content = std::string(
				(std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>()
			);
		}
		else
		{
			useOtherConfig = false;
		}

		loadtime(useOtherConfig, content);
		return 0;
	}

	// Check for Foldersize
	if (std::string(argv[1]) == "--foldersize" || std::string(argv[1]) == "-f")
	{
		std::string path = ".";
		if (argc > 2)
		{
			path = argv[2];
		}
		foldersize(path);
		return 0;
	}
}
