#pragma once

#include <iostream>
#include <random>
#include <chrono>

#include "../sx_lib/sx_lib.hpp"
#include "sx-multi_config.hpp"


// Function to print the help message for the Multi Tool
int printhelp();

// Function to run the number guessing game
void numberguess(int min, int max);

// Function to check the Time it took to load the config file
void loadtime(bool useOtherConfig, std::string content);
