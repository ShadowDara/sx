// edi 3.0
// Dynamischer Terminal-Editor in modernem C++
// By Shadowdara
// Apache-2.0 2026

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h> // Für _getch() auf Windows
#include <fstream>

std::vector<std::string> buffer;
int cur_x = 0;
int cur_y = 0;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
DWORD originalConsoleMode;

// Alternate Screen Buffer
HANDLE hAltBuffer;
HANDLE hMainBuffer;

void enable_alternate_screen() {
    hMainBuffer = hOut;
    hAltBuffer = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    SetConsoleActiveScreenBuffer(hAltBuffer);

    // Save original mode
    GetConsoleMode(hIn, &originalConsoleMode);
    DWORD mode = originalConsoleMode;
    mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(hIn, mode);
}

void disable_alternate_screen() {
    SetConsoleActiveScreenBuffer(hMainBuffer);
    SetConsoleMode(hIn, originalConsoleMode);
    CloseHandle(hAltBuffer);
}

void help() {
    std::cout << "Mini-Terminal-Editor Help\n"
        << "ESC = Exit, Ctrl+S = Save\n";
}

void move_cursor(int x, int y) {
    std::cout << "\x1b[" << (y + 1) << ";" << (x + 1) << "H";
}

// Zeichne nur den existierenden Text + Header einmal
void draw_screen() {
    COORD pos{ 0, 0 };
    SetConsoleCursorPosition(hAltBuffer, pos);
    std::cout << "Mini-Terminal-Editor (ESC = Exit, Ctrl+S = Save)\n";
    for (auto& line : buffer) {
        std::cout << line << std::string(80 - line.size(), ' ') << "\n"; // Zeile auffüllen
    }
    SetConsoleCursorPosition(hAltBuffer, { static_cast<SHORT>(cur_x), static_cast<SHORT>(cur_y + 1) });
}

bool is_line_empty(const std::string& line) {
    for (char c : line)
        if (c != ' ' && c != '\t') return false;
    return true;
}

void save_file(const std::string& filename) {
    std::ofstream out(filename);
    if (!out) return;

    int last_nonempty = buffer.size() - 1;
    for (int i = buffer.size() - 1; i >= 0; --i) {
        if (!is_line_empty(buffer[i])) {
            last_nonempty = i;
            break;
        }
    }

    for (int i = 0; i <= last_nonempty; ++i) {
        out << buffer[i] << "\n";
    }
}


int main(int argc, char* argv[]) {
    std::string filename = (argc > 1) ? argv[1] : "output.txt";

    std::ifstream in(filename);
    std::string line;
    while (std::getline(in, line)) buffer.push_back(line);
    if (buffer.empty()) buffer.push_back("");

    enable_alternate_screen();
    draw_screen();

    int ch;
    while (true) {
        ch = _getch();
        if (ch == 27) break;       // ESC
        else if (ch == 19) save_file(filename); // Ctrl+S
        else if (ch == 8) {        // Backspace
            if (cur_x > 0) {
                buffer[cur_y].erase(cur_x - 1, 1);
                --cur_x;
            }
            else if (cur_y > 0) {
                cur_x = buffer[cur_y - 1].size();
                buffer[cur_y - 1] += buffer[cur_y];
                buffer.erase(buffer.begin() + cur_y);
                --cur_y;
            }
        }
        else if (ch == 13) { // Enter
            std::string remainder = buffer[cur_y].substr(cur_x);
            buffer[cur_y] = buffer[cur_y].substr(0, cur_x);
            buffer.insert(buffer.begin() + cur_y + 1, remainder);
            ++cur_y;
            cur_x = 0;
        }
        else if (ch == 224) { // Arrow keys & DEL
            ch = _getch();
            if (ch == 72 && cur_y > 0) cur_y--; // Up
            else if (ch == 80 && cur_y + 1 < (int)buffer.size()) cur_y++; // Down
            else if (ch == 75 && cur_x > 0) cur_x--; // Left
            else if (ch == 77 && cur_x < (int)buffer[cur_y].size()) cur_x++; // Right
            else if (ch == 83 && cur_x < (int)buffer[cur_y].size()) buffer[cur_y].erase(cur_x, 1); // DEL
        }
        else if (ch >= 32 && ch <= 126) {
            buffer[cur_y].insert(buffer[cur_y].begin() + cur_x, (char)ch);
            ++cur_x;
        }
        draw_screen();
    }

    disable_alternate_screen();
    std::cout << "Datei gespeichert als " << filename << "\n";
}

