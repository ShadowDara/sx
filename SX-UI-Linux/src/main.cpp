#include "ui.hpp"
#include <iostream>
#include <vector>
#include <string>

static void activate(GtkApplication* app, gpointer user_data)
{
    auto window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "SX-UI Wrapper");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    auto grid = UI::CreateGrid(10, 10);

    // 16 Commands definieren
    std::vector<std::pair<std::string, std::function<void()>>> commands = {
        {"Applauncher", [](){ std::cout << "Applauncher gestartet\n"; }},
        {"Cmd 2", [](){ std::cout << "Command 2\n"; }},
        {"Cmd 3", [](){ std::cout << "Command 3\n"; }},
        {"Cmd 4", [](){ std::cout << "Command 4\n"; }},
        {"Cmd 5", [](){ std::cout << "Command 5\n"; }},
        {"Cmd 6", [](){ std::cout << "Command 6\n"; }},
        {"Cmd 7", [](){ std::cout << "Command 7\n"; }},
        {"Cmd 8", [](){ std::cout << "Command 8\n"; }},
        {"Cmd 9", [](){ std::cout << "Command 9\n"; }},
        {"Cmd 10", [](){ std::cout << "Command 10\n"; }},
        {"Cmd 11", [](){ std::cout << "Command 11\n"; }},
        {"Cmd 12", [](){ std::cout << "Command 12\n"; }},
        {"Cmd 13", [](){ std::cout << "Command 13\n"; }},
        {"Cmd 14", [](){ std::cout << "Command 14\n"; }},
        {"Cmd 15", [](){ std::cout << "Command 15\n"; }},
        {"Cmd 16", [](){ std::cout << "Command 16\n"; }},
    };

    int cols = 4;

    for (int i = 0; i < 16; i++) {
        auto btn = UI::CreateButton(commands[i].first.c_str(), commands[i].second);

        int row = i / cols;
        int col = i % cols;

        UI::GridAdd(grid, btn, col, row);
    }

    UI::SetChild(window, grid);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char** argv)
{
    GtkApplication* app = gtk_application_new("com.example.gridapp", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
