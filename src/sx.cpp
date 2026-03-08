#define KVP_IMPLEMENTATION
#include "../sx_lib/KVP_single_header.hpp"

#include "sx.hpp"


// Function to print the help message
int printhelp()
{
	std::cout
		<< GREEN "Usage: shortcuter [Command Shortcut] {Arguments ...}\n" END
		<< "\n"
		<< YELLOW "Example Configuration File:\n" END
		<< "\n"
		<< "# Config File for shortcuter\n"
		<< "# \n"
		<< "# Lines which start with #, are comments!\n"
		<< "# \n"
		<< "# Add KeyValuePairs here not for command shortcuts\n"
		<< "# The left is the shortcut and the right is the long command\n"
		<< "# which will be executed!\n"
		<< "#\n"
		<< "# All Arguments after the the shortcut command will be added\n"
		<< "# to the runned command!\n"
		<< "#\n"
		<< "hi=echo Hallo World\n"
		<< "\n"
		<< CYAN "[OPTIONS]" END "\n"
		<< "--view, -vi\tView all shortcuts\n"
		<< "--version, -v\tprint the Version\n"
		<< "--info, -i\tget Info about the Program Data\n"
		<< "--command, -c\tstart a command\n"
	;
	return 0;
}


// Function to print Info
int printinfo()
{
	std::cout << "SX by Shadowdara Version " << Version << " Built at " << BuildTime << "\n";
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
	std::unordered_map<std::string, std::string>& result)
{
	// Check if shortcut exists
	auto it = result.find(argv[1]);
	if (it == result.end())
	{
		std::cout << "Shortcut not found: " << argv[1] << "\n";
		return 1;
	}

	// Base command from config
	std::string command = it->second;

	// Check if its a reserved Keyword
	for (auto entry: ReservedCommands)
	{
		// std::cout << entry << " ! " << it->first << "\n";
		if (entry == it->first)
		{
			std::cout << RED << "[ERROR] " << YELLOW << it->first
				<< END << " is a reserved Keyword! It can not be used as Shortcut!\n";
			return 1;
		}
	}

	// Append additional arguments
	for (int i = 2; i < argc; ++i)
	{
		command += " ";
		command += argv[i];
	}

	// Execute
	auto returncode = runCommand(
		command,
		result["--windows-default-shell"],
		result["--linux-default-shell"]
	);

	//printf("Command returned with code %d\n", returncode);

	return returncode;
}


// Function to check if the Defaul Message with no arguments go
// Overwritten
bool checkNewStartMessage(std::unordered_map<std::string, std::string>& result)
{
	// Check for Standard print overwriting
	auto it = result.find("--overwrite-start-message");
	if (it == result.end())
	{
		return false;
	}

	// Set in the Config to true
	std::string OverrwriteStartMessage = it->second;
	if (OverrwriteStartMessage == "true")
	{
		return true;
	}

	return false;
}


// Function to print the new Start Message
// while be first searched for the file path from the hash map and then
// the File path will be loaded, and file content will be written to the
// Terminal
void printNewStartMessage(std::unordered_map<std::string, std::string>& result)
{
	auto it = result.find("--start-message-file");
	if (it == result.end())
	{
		std::cout << "Start Message File Entry not found!\n";
		return;
	}

	std::string NewStartMessageFile = it->second;

	std::ifstream file(getHomeDirectory() + "/" + NewStartMessageFile); // Datei öffnen

	if (!file) { // Prüfen, ob die Datei existiert
        std::cerr << "Error while opening the File " << NewStartMessageFile
			<< "! Does it exist?\n";
        return;
    }

    std::string line;
    while (std::getline(file, line))
	{
		// Zeilenweise lesen
        std::cout << line << '\n';      // In die Konsole ausgeben
    }

    file.close(); // optional, da Destruktor automatisch schließt

	auto it2 = result.find("--add-info-to-start-message");
	if (it2 == result.end())
	{
		return;
	}

	if (it2->second == "true")
	{
		std::cout << "SX Version " Version " - BuildTime " BuildTime "\n";
	}

    return;
}


// // Function to change a Command
// int changeCommand(int argc, char* argv[])
// {
// 	// 0 Name
// 	// 1 call this
// 	// 2 Mode
// 	// 3 Entryname

// 	if (argc < 4)
// 	{
// 		return 1;
// 	}

// 	std::string mode = argv[2];
// 	std::string entryname = argv[3];

// 	// Delete an Entry
// 	if (mode == "delete") {}

// 	// Append an Entry
// 	if (mode == "append") {}

// 	// Modify an Entry
// 	if (mode == "modify") {}
// }
