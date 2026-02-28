#pragma once

#include <vector>
#include <string>
#include <string_view>


// Config Struct for SX
struct Config {
    bool NewStartMessage = false;
};

// Reserved Commads
// Fixed Size because its known at compile Time
inline constexpr std::string_view ReservedCommands[3] = {
    "--overwrite-start-message",
    "--start-message-file",
    "--windows-default-shell"
};

// Version and ca Buildtime
inline constexpr std::string_view Version = "0.2.1";
inline constexpr std::string_view BuildTime = "March 2026";
