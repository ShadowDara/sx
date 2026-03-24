#pragma once
#include <gtk/gtk.h>
#include <functional>

namespace UI {

inline void Init(int* argc, char*** argv) {
    gtk_init();
}

inline GtkWidget* CreateWindow(const char* title, int width, int height) {
    GtkWidget* window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_window_destroy), NULL);
    return window;
}

inline GtkWidget* CreateButton(const char* text, std::function<void()> onClick) {
    GtkWidget* button = gtk_button_new_with_label(text);

    // Lambda → C callback bridge
    auto* func = new std::function<void()>(onClick);

    g_signal_connect(button, "clicked", G_CALLBACK(+[] (GtkWidget*, gpointer data) {
        auto* f = static_cast<std::function<void()>*>(data);
        (*f)();
    }), func);

    return button;
}

inline GtkWidget* CreateVBox(int spacing = 5) {
    return gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
}

inline void Add(GtkWidget* parent, GtkWidget* child) {
    gtk_box_append(GTK_BOX(parent), child);
}

inline void SetChild(GtkWidget* window, GtkWidget* child) {
    gtk_window_set_child(GTK_WINDOW(window), child);
}

inline void Show(GtkWidget* widget) {
    gtk_widget_set_visible(widget, TRUE);
}

inline GtkWidget* CreateGrid(int row_spacing = 5, int col_spacing = 5) {
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), row_spacing);
    gtk_grid_set_column_spacing(GTK_GRID(grid), col_spacing);
    return grid;
}

inline void GridAdd(GtkWidget* grid, GtkWidget* child, int col, int row, int width = 1, int height = 1) {
    gtk_grid_attach(GTK_GRID(grid), child, col, row, width, height);
}

}