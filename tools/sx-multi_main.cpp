// Main file for the Multi Tool

#include <iostream>

#include "../src/sx-multi_config.hpp"
#include "../src/sx-multi.hpp"


// Main function for the Multi Tool
int main(int argc, char* argv[])
{
	MultiConfig config;
	
	// Check if Argumentsare here
	if (argc < 2)
	{
		std::cout << "Please run with at least one argument or --help or -h.\n";
		return 1;
	}
}
