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
inline constexpr std::string_view ReservedCommands[5] = {
    "--overwrite-start-message",
    "--start-message-file",
    "--windows-default-shell",
    "--linux-default-shell",
    "--add-info-to-start-message"
};

// Version and ca Buildtime
#define Version "0.2.1"
#define BuildTime "March 2026"
