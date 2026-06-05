#include <sfplib/parser.hpp>

namespace fs = std::filesystem;

using Tasks = std::unordered_map<std::string, Task>;

// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------

static inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}


// ------------------------------------------------------------
// Cycle Detection
// ------------------------------------------------------------

void detect_cycles(
    const Tasks& tasks,
    const std::string& name,
    std::unordered_map<std::string, VisitState>& state,
    std::vector<std::string>& stack
) {
    VisitState current =
        state.count(name)
            ? state[name]
            : VisitState::NotVisited;

    switch (current) {
        case VisitState::Visiting: {
            // CYCLE FOUND
            auto it = std::find(stack.begin(), stack.end(), name);

            if (it == stack.end()) {
                throw std::runtime_error(
                    "Internal error: cycle detection state corrupted"
                );
            }

            std::cout << "samfile Cycle detected: ";

            for (; it != stack.end(); ++it) {
                std::cout << *it << " ";
            }

            std::cout << std::endl;

            throw std::runtime_error("Cycle detected!");
        }

        case VisitState::Visited:
            return;

        case VisitState::NotVisited:
            break;
    }

    // mark as visiting
    state[name] = VisitState::Visiting;
    stack.push_back(name);

    auto task_it = tasks.find(name);

    if (task_it == tasks.end()) {
        throw std::runtime_error("task not found");
    }

    const Task& task = task_it->second;

    // Check Unknown dependencies
    for (const auto& dep : task.deps) {
        if (!tasks.count(dep)) {
            throw std::runtime_error(
                "Unknown dependency '" + dep +
                "' in task '" + name + "'"
            );
        }
    }

    for (const auto& dep : task.deps) {
        detect_cycles(tasks, dep, state, stack);
    }

    stack.pop_back();
    state[name] = VisitState::Visited;
}


// To detect Cycles
void validate_all(const Tasks& tasks) {
    std::unordered_map<std::string, VisitState> state;
    std::vector<std::string> stack;

    for (const auto& [task, _] : tasks) {
        detect_cycles(tasks, task, state, stack);
    }
}


// ------------------------------------------------------------
// Parse Commands
// ------------------------------------------------------------

// Function to parse a Line
std::optional<Command> parse_line(const std::string& raw_line) {
    std::string line = trim(raw_line);

    if (line.rfind("cd ", 0) == 0) {
        return Command{
            CommandType::Cd,
            line.substr(3),
            ""
        };
    }

    if (line.rfind("run ", 0) == 0) {
        std::string cmd = trim(line.substr(4));

        if (cmd.empty()) {
            throw std::runtime_error("Invalid empty run command");
        }

        return Command{
            CommandType::Run,
            cmd,
            ""
        };
    }

    if (line.rfind("env ", 0) == 0) {
        // env KEY=VALUE
        std::string rest = line.substr(4);

        size_t eq = rest.find('=');

        if (eq != std::string::npos) {
            return Command{
                CommandType::Env,
                rest.substr(0, eq),
                rest.substr(eq + 1)
            };
        }
    }

    return std::nullopt;
}


// Function to parse the Header of a Task
std::pair<std::string, std::vector<std::string>>
parse_task_header(const std::string& line) {
    size_t colon = line.find(':');

    std::string name =
        trim(line.substr(0, colon));

    std::vector<std::string> deps;

    if (colon != std::string::npos) {
        std::stringstream ss(line.substr(colon + 1));

        std::string dep;

        while (ss >> dep) {
            deps.push_back(dep);
        }
    }

    return {name, deps};
}


// ------------------------------------------------------------
// Parse File
// ------------------------------------------------------------

// Function to Parse the File
Tasks parse(const std::string& content) {
    Tasks tasks;

    std::optional<std::string> current;

    std::stringstream ss(content);

    std::string line;

    while (std::getline(ss, line)) {
        while (!line.empty() &&
               (line.back() == '\n' || line.back() == '\r')) {
            line.pop_back();
        }

        std::string trimmed = trim(line);

        // ignore empty lines
        if (trimmed.empty()) {
            continue;
        }

        // ignore comments
        if (trimmed.rfind("#", 0) == 0 ||
            trimmed.rfind("//", 0) == 0 ||
            trimmed.rfind("--", 0) == 0) {
            continue;
        }

        // task header
        if (!line.empty() &&
            line[0] != ' ' &&
            line.find(':') != std::string::npos) {

            auto [name, deps] = parse_task_header(line);

            tasks[name] = Task{
                deps,
                {}
            };

            current = name;
        }

        // command
        else if (!line.empty() && line[0] == ' ') {
            if (current.has_value()) {
                auto cmd = parse_line(line);

                if (cmd.has_value()) {
                    tasks[*current]
                        .commands
                        .push_back(*cmd);
                } else {
                    // ignore unknown lines
                    std::cerr
                        << "warning: ignored invalid line: "
                        << line
                        << std::endl;
                }
            }
        }
    }

    return tasks;
}


// ------------------------------------------------------------
// Run Tasks
// ------------------------------------------------------------

// Function to run a Task
void run_task(
    const Tasks& tasks,
    const std::string& name,
    std::unordered_set<std::string>& visited,
    RuntimeState& state
) {
    if (visited.count(name)) {
        return;
    }

    visited.insert(name);

    auto task_it = tasks.find(name);

    if (task_it == tasks.end()) {
        std::string msg =
            "Task '" + name + "' not found\n";

        msg += "Available tasks:\n";

        for (const auto& [key, _] : tasks) {
            msg += "  - " + key + "\n";
        }

        std::cout << "\n" << msg << std::endl;

        // no panic on the titanic
        return;
    }

    const Task& task = task_it->second;

    RuntimeState local_state{
        state.cwd,
        state.env
    };

    // 1. run dependencies first
    for (const auto& dep : task.deps) {
        run_task(tasks, dep, visited, local_state);
    }

    std::cout << "\n==> running task: "
              << name
              << "\n"
              << std::endl;

    // 2. run commands
    for (const auto& cmd : task.commands) {
        switch (cmd.type) {

            case CommandType::Cd: {
                fs::path new_path;

                if (fs::path(cmd.a).is_absolute()) {
                    new_path = fs::path(cmd.a);
                } else {
                    new_path = local_state.cwd / cmd.a;
                }

                std::cout
                    << "> cd "
                    << new_path.string()
                    << std::endl;

                if (!fs::exists(new_path)) {
                    throw std::runtime_error(
                        "cd failed: path does not exist: " +
                        new_path.string()
                    );
                }

                local_state.cwd =
                    fs::canonical(new_path);

                break;
            }

            case CommandType::Env: {
                local_state.env[cmd.a] = cmd.b;
                break;
            }

            case CommandType::Run: {
                run_task_command(local_state, cmd);
                break;
            }
            
            case CommandType::RunWin: {
#ifdef _WIN32
                run_task_command(local_state, cmd);
#endif
            }
            
            case CommandType::RunMac: {
#ifdef __APPLE__
                run_task_command(local_state, cmd);
#endif
            }
            
            case CommandType::RunLin: {
#ifdef __linux__
                run_task_command(local_state, cmd);
#endif
                break;
            }
        }
    }
}

void run_task_command(RuntimeState local_state, Command cmd) {
    std::string env_prefix;

    for (const auto& [k, v] : local_state.env) {
        env_prefix += k + "=\"" + v + "\" ";
    }

#ifdef _WIN32

    std::string full_cmd =
        "cd /d \"" + local_state.cwd.string() +
        "\" && " + env_prefix + cmd.a;

#else

    std::string full_cmd =
        "cd \"" + local_state.cwd.string() +
        "\" && " + env_prefix + cmd.a;

#endif

    std::cout
        << "> "
        << full_cmd
        << std::endl;

    int result = std::system(full_cmd.c_str());

    if (result != 0) {
        throw std::runtime_error(
            "task failed: " + cmd.a
        );
    }
}

void run_sam_file(const std::string& command) {

    RuntimeState state{
        fs::current_path(),
        {}
    };

    // Read samfile
    std::ifstream file(".samengine/samfile");

    if (!file.is_open()) {
        std::cerr
            << "Error while reading samfile"
            << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string content = buffer.str();

    // Parse tasks
    Tasks tasks = parse(content);

    // Check for cyclic dependencies
    validate_all(tasks);

    // Map which task was already visited
    std::unordered_set<std::string> visited;

    // Execute the task
    run_task(
        tasks,
        command,
        visited,
        state
    );
}
