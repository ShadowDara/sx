#include "ui.hpp"
#include <iostream>

static void activate(GtkApplication* app, gpointer user_data)
{
    auto window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Meine App");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    auto layout = UI::CreateVBox();

    auto button = UI::CreateButton("Klick mich", []() {
        std::cout << "Button geklickt!" << std::endl;
    });

    UI::Add(layout, button);
    UI::SetChild(window, layout);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char** argv)
{
    GtkApplication* app = gtk_application_new("com.example.myapp", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}