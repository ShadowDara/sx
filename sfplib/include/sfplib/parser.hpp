#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <optional>
#include <algorithm>

// Commands
enum class CommandType {
    Cd,
    Run,
    Env,
    
    // Command which runs only on windows
    RunWin,
    RunMac,
    RunLin,
};

struct Command {
    CommandType type;
    std::string a;
    std::string b; // only used for Env
};

struct Task {
    std::vector<std::string> deps;
    std::vector<Command> commands;
};

enum class VisitState {
    NotVisited,
    Visiting,
    Visited
};

// Runtime Path for every Script
struct RuntimeState {
    std::filesystem::path cwd;
    std::unordered_map<std::string, std::string> env;
};

static inline std::string trim(const std::string& s);

void detect_cycles(
    const std::unordered_map<std::string, Task>& tasks,
    const std::string& name,
    std::unordered_map<std::string, VisitState>& state,
    std::vector<std::string>& stack
);

void validate_all(const std::unordered_map<std::string, Task>& tasks);

std::optional<Command> parse_line(const std::string& raw_line);

std::pair<std::string, std::vector<std::string>> parse_task_header(const std::string& line);

std::unordered_map<std::string, Task> parse(const std::string& content);

void run_task(
    const std::unordered_map<std::string, Task>& tasks,
    const std::string& name,
    std::unordered_set<std::string>& visited,
    RuntimeState& state
);

void run_task_command(RuntimeState local_state, Command cmd);

void run_sam_file(const std::string& command);
