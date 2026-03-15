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


struct ConsoleChar {
    char ch;
    WORD attr;
};

std::vector<std::string> buffer;
int cur_x = 0;
int cur_y = 0;

DWORD originalConsoleMode;

std::vector<ConsoleChar> saved_screen;
COORD screen_size;
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

void save_screen() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    screen_size = csbi.dwSize;

    int total_cells = csbi.dwSize.X * csbi.dwSize.Y;
    saved_screen.resize(total_cells);

    CHAR_INFO* chars = new CHAR_INFO[total_cells];
    COORD bufSize = csbi.dwSize;
    COORD bufCoord{ 0,0 };
    SMALL_RECT readRegion{ 0,0,csbi.dwSize.X - 1, csbi.dwSize.Y - 1 };
    ReadConsoleOutput(hOut, chars, bufSize, bufCoord, &readRegion);

    for (int i = 0; i < total_cells; i++) {
        saved_screen[i].ch = chars[i].Char.AsciiChar;
        saved_screen[i].attr = chars[i].Attributes;
    }

    delete[] chars;
}

void restore_screen() {
    int total_cells = screen_size.X * screen_size.Y;
    CHAR_INFO* chars = new CHAR_INFO[total_cells];
    for (int i = 0; i < total_cells; i++) {
        chars[i].Char.AsciiChar = saved_screen[i].ch;
        chars[i].Attributes = saved_screen[i].attr;
    }
    COORD bufSize = screen_size;
    COORD bufCoord{ 0,0 };
    SMALL_RECT writeRegion{ 0,0,screen_size.X - 1,screen_size.Y - 1 };
    WriteConsoleOutput(hOut, chars, bufSize, bufCoord, &writeRegion);
    delete[] chars;
}

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
    COORD pos{ 0,0 };
    SetConsoleCursorPosition(hOut, pos);
    DWORD written;
    std::string header = "Mini-Terminal-Editor (ESC=Exit, Ctrl+S=Save)\n";
    WriteConsoleA(hOut, header.c_str(), header.size(), &written, NULL);
    for (auto& line : buffer) {
        std::string out_line = line + "\n";
        WriteConsoleA(hOut, out_line.c_str(), out_line.size(), &written, NULL);
    }
    SetConsoleCursorPosition(hOut, { static_cast<SHORT>(cur_x), static_cast<SHORT>(cur_y + 1) });
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

    // Screen sichern
    save_screen();

    // Editor Screen leeren
    system("cls");
    draw_screen();

    while (true) {
        INPUT_RECORD rec;
        DWORD n;
        ReadConsoleInput(hIn, &rec, 1, &n);
        if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown)
        {
            auto& key = rec.Event.KeyEvent;
            char ch = key.uChar.AsciiChar;

            if (key.wVirtualKeyCode == VK_ESCAPE)
            { 
                break;
            }
            else if (key.wVirtualKeyCode == 'S' && key.dwControlKeyState & LEFT_CTRL_PRESSED)
            {
                save_file(filename);
            }
            else if (ch >= 32 && ch <= 126)
            { // Printable
                buffer[cur_y].insert(buffer[cur_y].begin() + cur_x, ch);
                ++cur_x;
            }
            else if (key.wVirtualKeyCode == VK_BACK)
            {
                if (cur_x > 0)
                {
                    buffer[cur_y].erase(cur_x - 1, 1);
                    --cur_x;
                }
                else if (cur_y > 0)
                {
                    cur_x = buffer[cur_y - 1].size();
                    buffer[cur_y - 1] += buffer[cur_y];
                    buffer.erase(buffer.begin() + cur_y);
                    --cur_y;
                }
            }
            else if (key.wVirtualKeyCode == VK_RETURN)
            {
                std::string rem = buffer[cur_y].substr(cur_x);
                buffer[cur_y] = buffer[cur_y].substr(0, cur_x);
                buffer.insert(buffer.begin() + cur_y + 1, rem);
                ++cur_y;
                cur_x = 0;
            }
            else if (key.wVirtualKeyCode == VK_LEFT && cur_x > 0)
            {
                cur_x--;
            }
            else if (key.wVirtualKeyCode == VK_RIGHT && cur_x < (int)buffer[cur_y].size())
            {
                cur_x++;
            }
            else if (key.wVirtualKeyCode == VK_UP && cur_y > 0) cur_y--;
            else if (key.wVirtualKeyCode == VK_DOWN && cur_y + 1 < (int)buffer.size()) cur_y++;

            draw_screen();
        }
    }

    // Clear screen beim Beenden
    system("cls");
    std::cout << "Datei gespeichert als " << filename << "\n";
}
