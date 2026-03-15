/*
 * mined — Minimalistischer Terminal Editor (Windows, reine WinAPI)
 *
 * Steuerung:
 *   Pfeiltasten     Cursor bewegen
 *   Ctrl+S          Speichern
 *   Ctrl+Q          Beenden
 *   Ctrl+F          Suchen
 *   Ctrl+Z          Rueckgaengig
 *   Backspace/Del   Zeichen loeschen
 *   Enter           Neue Zeile
 *   PgUp/PgDn       Seite blaettern
 *   Home/End        Zeilenanfang/-ende
 *
 * Kompilieren (MSVC Developer-Prompt):
 *   cl /std:c++17 /O2 /EHsc mined.cpp /Fe:mined.exe
 *
 * Kompilieren (MinGW / g++):
 *   g++ -std=c++17 -O2 -o mined.exe mined.cpp
 *
 * Funktioniert in: cmd.exe, PowerShell, Windows Terminal
 * Benoetigt: Windows 7+ (keine ANSI/VT-Unterstuetzung noetig)
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

 // ─── Handles & gespeicherter Konsolenzustand ─────────────────────────────────

static HANDLE hOut = INVALID_HANDLE_VALUE; // unser eigener Screen-Buffer
static HANDLE hOutOld = INVALID_HANDLE_VALUE; // originaler Buffer (zum Restore)
static HANDLE hIn = INVALID_HANDLE_VALUE;
static DWORD  origInMode = 0;
static CONSOLE_CURSOR_INFO origCursor;

void restoreConsole() {
    if (hOutOld != INVALID_HANDLE_VALUE) {
        // Originalen Buffer wieder aktiv schalten → alter Terminal-Inhalt kommt zurück
        SetConsoleActiveScreenBuffer(hOutOld);
        SetConsoleCursorInfo(hOutOld, &origCursor);
    }
    if (hOut != INVALID_HANDLE_VALUE && hOut != hOutOld)
        CloseHandle(hOut);
    if (hIn != INVALID_HANDLE_VALUE)
        SetConsoleMode(hIn, origInMode);
}

void setupConsole() {
    hIn = GetStdHandle(STD_INPUT_HANDLE);
    hOutOld = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleMode(hIn, &origInMode);
    GetConsoleCursorInfo(hOutOld, &origCursor);

    // Eigenen Screen-Buffer erstellen — komplett leer, kein alter Inhalt sichtbar
    hOut = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        CONSOLE_TEXTMODE_BUFFER,
        nullptr
    );

    // Diesen Buffer aktiv schalten → der alte Terminal-Output ist weg
    SetConsoleActiveScreenBuffer(hOut);

    // Puffergröße == Fenstergröße → kein Scrollbalken, kein hochscrollen möglich
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        int winCols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int winRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        SMALL_RECT win = { 0, 0, (SHORT)(winCols - 1), (SHORT)(winRows - 1) };
        COORD      buf = { (SHORT)winCols, (SHORT)winRows };
        SetConsoleWindowInfo(hOut, TRUE, &win);
        SetConsoleScreenBufferSize(hOut, buf);
    }

    // Roheingabe
    SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
    SetConsoleMode(hOut, ENABLE_PROCESSED_OUTPUT);

    SetConsoleOutputCP(CP_ACP);
    SetConsoleCP(CP_ACP);

    atexit(restoreConsole);
}

// ─── Hilfsfunktionen Konsole ─────────────────────────────────────────────────

void getWindowSize(int& rows, int& cols) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    else {
        rows = 24; cols = 80;
    }
}

void setCursorPos(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, c);
}

void setCursorVisible(bool visible) {
    CONSOLE_CURSOR_INFO ci;
    ci.dwSize = 20;
    ci.bVisible = visible ? TRUE : FALSE;
    SetConsoleCursorInfo(hOut, &ci);
}

// Eine Zeile an Position (row) mit gegebenem Text und Attribut schreiben.
// Füllt die gesamte Zeile auf screenCols auf.
void writeLine(int row, int screenCols, const std::string& text, WORD attr) {
    COORD coord = { 0, (SHORT)row };
    DWORD written;

    // Puffer mit Leerzeichen auffüllen
    std::string buf = text;
    if ((int)buf.size() < screenCols)
        buf += std::string(screenCols - buf.size(), ' ');
    else
        buf = buf.substr(0, screenCols);

    WriteConsoleOutputCharacterA(hOut, buf.c_str(), (DWORD)buf.size(), coord, &written);
    FillConsoleOutputAttribute(hOut, attr, (DWORD)buf.size(), coord, &written);
}

// Gesamten sichtbaren Bereich leeren
void clearScreen(int rows, int cols) {
    COORD origin = { 0, 0 };
    DWORD written;
    WORD  attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    FillConsoleOutputCharacterA(hOut, ' ', (DWORD)(rows * cols), origin, &written);
    FillConsoleOutputAttribute(hOut, attr, (DWORD)(rows * cols), origin, &written);
}

// ─── Tastencodes ─────────────────────────────────────────────────────────────

enum Key {
    KEY_CTRL_F = 6,
    KEY_CTRL_Q = 17,
    KEY_CTRL_S = 19,
    KEY_CTRL_Z = 26,
    KEY_ENTER = 13,
    KEY_BACKSPACE = 8,
    KEY_ESCAPE = 27,
    KEY_ARROW_UP = 1000,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,
    KEY_HOME,
    KEY_END,
    KEY_DEL,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
};

int readKey() {
    while (true) {
        INPUT_RECORD ir;
        DWORD count = 0;
        if (!ReadConsoleInputA(hIn, &ir, 1, &count) || count == 0) continue;

        // Fenstergröße geändert → ignorieren (Editor re-rendert beim nächsten Tastendruck)
        if (ir.EventType == WINDOW_BUFFER_SIZE_EVENT) return -1;
        if (ir.EventType != KEY_EVENT)                continue;
        if (!ir.Event.KeyEvent.bKeyDown)              continue;

        WORD vk = ir.Event.KeyEvent.wVirtualKeyCode;
        DWORD mod = ir.Event.KeyEvent.dwControlKeyState;
        char  ch = ir.Event.KeyEvent.uChar.AsciiChar;

        bool ctrl = (mod & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;

        if (ctrl) {
            switch (vk) {
            case 'S': return KEY_CTRL_S;
            case 'Q': return KEY_CTRL_Q;
            case 'F': return KEY_CTRL_F;
            case 'Z': return KEY_CTRL_Z;
            }
        }

        switch (vk) {
        case VK_UP:     return KEY_ARROW_UP;
        case VK_DOWN:   return KEY_ARROW_DOWN;
        case VK_LEFT:   return KEY_ARROW_LEFT;
        case VK_RIGHT:  return KEY_ARROW_RIGHT;
        case VK_HOME:   return KEY_HOME;
        case VK_END:    return KEY_END;
        case VK_DELETE: return KEY_DEL;
        case VK_PRIOR:  return KEY_PAGE_UP;
        case VK_NEXT:   return KEY_PAGE_DOWN;
        case VK_RETURN: return KEY_ENTER;
        case VK_BACK:   return KEY_BACKSPACE;
        case VK_ESCAPE: return KEY_ESCAPE;
        }

        if (ch >= 32 && ch < 127) return (unsigned char)ch;
    }
}

// ─── Undo ────────────────────────────────────────────────────────────────────

struct UndoEntry {
    std::vector<std::string> lines;
    int cx, cy;
};

// ─── Editor ──────────────────────────────────────────────────────────────────

struct Editor {
    std::vector<std::string> lines;
    int cx = 0, cy = 0;
    int rowoff = 0, coloff = 0;
    int screenRows = 24, screenCols = 80;
    bool dirty = false;
    std::string filename;
    std::string statusMsg;
    std::vector<UndoEntry> undoStack;

    // Farbattribute
    WORD attrNormal = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    WORD attrTilde = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    WORD attrStatus = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    WORD attrHelp = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    WORD attrPrompt = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    Editor() { lines.push_back(""); }

    // ── Datei ────────────────────────────────────────────────────────────────
    void open(const std::string& fname) {
        filename = fname;
        lines.clear();
        std::ifstream f(fname);
        if (f.is_open()) {
            std::string l;
            while (std::getline(f, l)) {
                if (!l.empty() && l.back() == '\r') l.pop_back();
                lines.push_back(l);
            }
        }
        if (lines.empty()) lines.push_back("");
        dirty = false;
    }

    bool save() {
        if (filename.empty()) {
            filename = promptInput("Dateiname: ");
            if (filename.empty()) { setStatus("Abgebrochen."); return false; }
        }
        std::ofstream f(filename);
        if (!f.is_open()) { setStatus("Fehler beim Speichern!"); return false; }
        for (size_t i = 0; i < lines.size(); i++) {
            f << lines[i];
            if (i + 1 < lines.size()) f << "\r\n";
        }
        dirty = false;
        setStatus("Gespeichert: " + filename);
        return true;
    }

    // ── Undo ─────────────────────────────────────────────────────────────────
    void pushUndo() {
        if (undoStack.size() >= 50) undoStack.erase(undoStack.begin());
        undoStack.push_back({ lines, cx, cy });
    }

    void undo() {
        if (undoStack.empty()) { setStatus("Nichts rueckgaengig."); return; }
        auto& e = undoStack.back();
        lines = e.lines; cx = e.cx; cy = e.cy;
        undoStack.pop_back();
        dirty = true;
        setStatus("Rueckgaengig.");
    }

    // ── Cursor ───────────────────────────────────────────────────────────────
    void moveCursor(int key) {
        switch (key) {
        case KEY_ARROW_LEFT:
            if (cx > 0) cx--;
            else if (cy > 0) { cy--; cx = (int)lines[cy].size(); }
            break;
        case KEY_ARROW_RIGHT:
            if (cx < (int)lines[cy].size()) cx++;
            else if (cy < (int)lines.size() - 1) { cy++; cx = 0; }
            break;
        case KEY_ARROW_UP:
            if (cy > 0) { cy--; cx = std::min(cx, (int)lines[cy].size()); }
            break;
        case KEY_ARROW_DOWN:
            if (cy < (int)lines.size() - 1) { cy++; cx = std::min(cx, (int)lines[cy].size()); }
            break;
        case KEY_HOME:     cx = 0; break;
        case KEY_END:      cx = (int)lines[cy].size(); break;
        case KEY_PAGE_UP:
            cy = std::max(0, cy - (screenRows - 2));
            cx = std::min(cx, (int)lines[cy].size()); break;
        case KEY_PAGE_DOWN:
            cy = std::min((int)lines.size() - 1, cy + (screenRows - 2));
            cx = std::min(cx, (int)lines[cy].size()); break;
        }
    }

    // ── Bearbeiten ───────────────────────────────────────────────────────────
    void insertChar(char c) {
        pushUndo();
        lines[cy].insert(cx, 1, c);
        cx++; dirty = true;
    }

    void insertNewline() {
        pushUndo();
        std::string rest = lines[cy].substr(cx);
        lines[cy].erase(cx);
        lines.insert(lines.begin() + cy + 1, rest);
        cy++; cx = 0; dirty = true;
    }

    void deleteChar() {
        if (cx == 0 && cy == 0) return;
        pushUndo();
        if (cx > 0) { lines[cy].erase(cx - 1, 1); cx--; }
        else {
            int nx = (int)lines[cy - 1].size();
            lines[cy - 1] += lines[cy];
            lines.erase(lines.begin() + cy);
            cy--; cx = nx;
        }
        dirty = true;
    }

    void deleteCharForward() {
        if (cx < (int)lines[cy].size()) {
            pushUndo(); lines[cy].erase(cx, 1); dirty = true;
        }
        else if (cy < (int)lines.size() - 1) {
            pushUndo(); lines[cy] += lines[cy + 1];
            lines.erase(lines.begin() + cy + 1); dirty = true;
        }
    }

    // ── Suchen ───────────────────────────────────────────────────────────────
    void search() {
        std::string q = promptInput("Suchen: ");
        if (q.empty()) return;
        for (int i = cy; i < (int)lines.size(); i++) {
            size_t start = (i == cy) ? (size_t)(cx + 1) : 0;
            size_t pos = lines[i].find(q, start);
            if (pos != std::string::npos) { cy = i; cx = (int)pos; setStatus("Gefunden."); return; }
        }
        for (int i = 0; i <= cy; i++) {
            size_t pos = lines[i].find(q);
            if (pos != std::string::npos) { cy = i; cx = (int)pos; setStatus("Gefunden (von vorne)."); return; }
        }
        setStatus("Nicht gefunden: " + q);
    }

    // ── Prompt ───────────────────────────────────────────────────────────────
    std::string promptInput(const std::string& msg) {
        std::string buf;
        while (true) {
            // Prompt-Zeile rendern
            std::string display = msg + buf + "_";
            writeLine(screenRows, screenCols, display, attrPrompt);
            setCursorPos((int)(msg.size() + buf.size()), screenRows);

            int k = readKey();
            if (k == KEY_ENTER)              break;
            if (k == KEY_ESCAPE) { buf.clear(); break; }
            if (k == KEY_BACKSPACE && !buf.empty()) buf.pop_back();
            else if (k >= 32 && k < 127)    buf += (char)k;
        }
        return buf;
    }

    void setStatus(const std::string& msg) { statusMsg = msg; }

    // ── Scroll ───────────────────────────────────────────────────────────────
    void scroll() {
        if (cy < rowoff) rowoff = cy;
        if (cy >= rowoff + screenRows - 1) rowoff = cy - screenRows + 2;
        if (cx < coloff) coloff = cx;
        if (cx >= coloff + screenCols) coloff = cx - screenCols + 1;
    }

    // ─── Render ──────────────────────────────────────────────────────────────
    void render() {
        // Konsolengröße neu abfragen (bei Resize)
        int newRows, newCols;
        getWindowSize(newRows, newCols);
        if (newRows != screenRows + 2 || newCols != screenCols) {
            screenRows = newRows - 2;
            screenCols = newCols;
            // Puffergröße auf neue Fenstergröße anpassen
            COORD      newBuf = { (SHORT)newCols, (SHORT)newRows };
            SMALL_RECT win = { 0, 0, (SHORT)(newCols - 1), (SHORT)(newRows - 1) };
            SetConsoleWindowInfo(hOut, TRUE, &win);
            SetConsoleScreenBufferSize(hOut, newBuf);
        }

        scroll();
        setCursorVisible(false);

        // ── Textzeilen ───────────────────────────────────────────────────────
        for (int y = 0; y < screenRows - 1; y++) {
            int fileRow = y + rowoff;
            if (fileRow < (int)lines.size()) {
                const std::string& l = lines[fileRow];
                int start = std::min(coloff, (int)l.size());
                std::string vis = l.substr(start);
                writeLine(y, screenCols, vis, attrNormal);
            }
            else {
                // Tilde für Leerzeilen
                COORD coord = { 0, (SHORT)y };
                DWORD written;
                std::string empty(screenCols, ' ');
                empty[0] = '~';
                WriteConsoleOutputCharacterA(hOut, empty.c_str(), (DWORD)screenCols, coord, &written);
                FillConsoleOutputAttribute(hOut, attrNormal, (DWORD)screenCols, coord, &written);
                // Tilde in Blau einfärben
                COORD tc = { 0, (SHORT)y };
                FillConsoleOutputAttribute(hOut, attrTilde, 1, tc, &written);
            }
        }

        // ── Statusleiste ─────────────────────────────────────────────────────
        {
            std::string left = " " + (filename.empty() ? "[Neue Datei]" : filename);
            if (dirty) left += " [*]";
            std::string right = std::to_string(cy + 1) + ":" + std::to_string(cx + 1) + " ";
            std::string mid = statusMsg;
            statusMsg.clear();

            std::string status = left;
            int pad = screenCols - (int)left.size() - (int)right.size();
            if (pad > 0 && !mid.empty()) {
                int mpad = (pad - (int)mid.size()) / 2 + (int)left.size();
                while ((int)status.size() < mpad) status += ' ';
                status += mid;
            }
            while ((int)status.size() < screenCols - (int)right.size()) status += ' ';
            status += right;
            writeLine(screenRows - 1, screenCols, status, attrStatus);
        }

        // ── Hilfezeile ───────────────────────────────────────────────────────
        writeLine(screenRows, screenCols,
            " ^S Speichern  ^Q Beenden  ^F Suchen  ^Z Rueckgaengig",
            attrHelp);

        // ── Cursor ───────────────────────────────────────────────────────────
        int screenX = cx - coloff;
        int screenY = cy - rowoff;
        setCursorPos(screenX, screenY);
        setCursorVisible(true);
    }

    // ─── Hauptschleife ───────────────────────────────────────────────────────
    void run() {
        getWindowSize(screenRows, screenCols);
        screenRows -= 2;

        clearScreen(screenRows + 2, screenCols);

        while (true) {
            render();
            int k = readKey();
            if (k == -1) continue; // Resize-Event, einfach neu rendern

            switch (k) {
            case KEY_CTRL_Q:
                if (dirty) {
                    std::string ans = promptInput("Aenderungen nicht gespeichert! Beenden? (j/n): ");
                    if (ans != "j" && ans != "J") { setStatus("Abgebrochen."); break; }
                }
                clearScreen(screenRows + 2, screenCols);
                setCursorPos(0, 0);
                setCursorVisible(true);
                exit(0);

            case KEY_CTRL_S:    save();             break;
            case KEY_CTRL_F:    search();           break;
            case KEY_CTRL_Z:    undo();             break;
            case KEY_ENTER:     insertNewline();    break;
            case KEY_BACKSPACE: deleteChar();       break;
            case KEY_DEL:       deleteCharForward(); break;

            case KEY_ARROW_UP:
            case KEY_ARROW_DOWN:
            case KEY_ARROW_LEFT:
            case KEY_ARROW_RIGHT:
            case KEY_HOME:
            case KEY_END:
            case KEY_PAGE_UP:
            case KEY_PAGE_DOWN:
                moveCursor(k);
                break;

            default:
                if (k >= 32 && k < 127) insertChar((char)k);
                break;
            }
        }
    }
};

// ─── Einstiegspunkt ──────────────────────────────────────────────────────────

int main(int argc, char* argv[]) {
    setupConsole();
    Editor ed;

    if (argc >= 2) {
        ed.open(argv[1]);
        ed.setStatus("\"" + std::string(argv[1]) + "\" geladen.");
    }
    else {
        ed.setStatus("Neue Datei -- Ctrl+S zum Speichern.");
    }

    ed.run();
    return 0;
}
