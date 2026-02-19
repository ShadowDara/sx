#include "oscore.hpp"

// Function to get the user's home directory in a cross-platform way
std::string getHomeDirectory()
{
#ifdef _WIN32
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif

    if (home)
    {
        return std::string(home);
    }

    return "";
}

// Function to run a command in the system shell
int runCommand(const std::string& command)
{
    return std::system(command.c_str());
}
