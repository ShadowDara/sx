#include "sx-multi.hpp"


// Function to print the help message for the Multi Tool
int printhelp()
{
	std::cout 
		<< "This is the Multi Tool. It is a collection of small tools and games.\n"
		<< "Currently it includes:\n"
		<< " - --numberguess, -n: A simple number guessing game.\n"
		<< "Usage:\n"
		<< " - Run the program and follow the instructions for the selected tool or game.\n"
	;
	return 0;
}


// Function to run the number guessing game
void numberguess(int min, int max)
{
	// Gen random Number
	std::random_device rd;
	std::mt19937 gen(rd()); // Zufallsgenerator

	std::uniform_int_distribution<> dist(min, max); // Zahlen von 1 bis 1000

	int zufall = dist(gen);

	std::cout << "Guess the Number between " << min << " and " << max << " \n";

	while (true)
	{
		int zahl;
		std::cin >> zahl;

		if (zahl == zufall)
		{
			std::cout << "Correct! The number was " << zufall << ".\n";
			break;
		}
		else if (zahl < zufall)
		{
			std::cout << "Too low! Try again.\n";
		}
		else
		{
			std::cout << "Too high! Try again.\n";
		}
	}
}
