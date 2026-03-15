/*
 * mined — Minimalistischer Terminal Editor
 * Steuerung:
 *   Pfeiltasten     — Cursor bewegen
 *   Ctrl+S          — Speichern
 *   Ctrl+Q          — Beenden (warnt bei ungespeicherten Änderungen)
 *   Ctrl+F          — Suchen
 *   Ctrl+Z          — Rückgängig
 *   Backspace/Del   — Zeichen löschen
 *   Enter           — Neue Zeile
 *   PgUp/PgDn       — Seite blättern
 *   Home/End        — Zeilenanfang/-ende
 *
 * Kompilieren: g++ -std=c++17 -O2 -o mined editor.cpp
 * Starten:     ./mined [datei]
 */

#include <algorithm>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

 // ─── Terminal-Rohmodus ───────────────────────────────────────────────────────

struct termios orig_termios;

void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// ─── Tastencodes ─────────────────────────────────────────────────────────────

enum Key {
    KEY_BACKSPACE = 127,
    KEY_ARROW_LEFT = 1000,
    KEY_ARROW_RIGHT,
    KEY_ARROW_UP,
    KEY_ARROW_DOWN,
    KEY_DEL,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_CTRL_S = 19,
    KEY_CTRL_Q = 17,
    KEY_CTRL_F = 6,
    KEY_CTRL_Z = 26,
    KEY_ENTER = 13,
    KEY_ESCAPE = 27,
};

int readKey() {
    char c;
    while (read(STDIN_FILENO, &c, 1) != 1);
    if (c != '\x1b') return (unsigned char)c;

    char seq[4] = {};
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return KEY_ESCAPE;
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return KEY_ESCAPE;

    if (seq[0] == '[') {
        if (seq[1] >= '0' && seq[1] <= '9') {
            if (read(STDIN_FILENO, &seq[2], 1) != 1) return KEY_ESCAPE;
            if (seq[2] == '~') {
                switch (seq[1]) {
                case '1': return KEY_HOME;
                case '3': return KEY_DEL;
                case '4': return KEY_END;
                case '5': return KEY_PAGE_UP;
                case '6': return KEY_PAGE_DOWN;
                case '7': return KEY_HOME;
                case '8': return KEY_END;
                }
            }
        }
        else {
            switch (seq[1]) {
            case 'A': return KEY_ARROW_UP;
            case 'B': return KEY_ARROW_DOWN;
            case 'C': return KEY_ARROW_RIGHT;
            case 'D': return KEY_ARROW_LEFT;
            case 'H': return KEY_HOME;
            case 'F': return KEY_END;
            }
        }
    }
    else if (seq[0] == 'O') {
        switch (seq[1]) {
        case 'H': return KEY_HOME;
        case 'F': return KEY_END;
        }
    }
    return KEY_ESCAPE;
}

// ─── Terminalgröße ───────────────────────────────────────────────────────────

void getWindowSize(int& rows, int& cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        rows = 24; cols = 80;
    }
    else {
        rows = ws.ws_row;
        cols = ws.ws_col;
    }
}

// ─── Undo-Eintrag ────────────────────────────────────────────────────────────

struct UndoEntry {
    std::vector<std::string> lines;
    int cx, cy;
};

// ─── Editor-Zustand ──────────────────────────────────────────────────────────

struct Editor {
    std::vector<std::string> lines;
    int cx = 0, cy = 0;          // Cursor (Spalte, Zeile)
    int rowoff = 0, coloff = 0;  // Scroll-Offset
    int screenRows = 24, screenCols = 80;
    bool dirty = false;
    std::string filename;
    std::string statusMsg;
    std::string searchQuery;
    std::vector<UndoEntry> undoStack;

    Editor() { lines.push_back(""); }

    // ── Datei laden ─────────────────────────────────────────────────────────
    void open(const std::string& fname) {
        filename = fname;
        lines.clear();
        std::ifstream f(fname);
        if (f.is_open()) {
            std::string l;
            while (std::getline(f, l)) lines.push_back(l);
            if (lines.empty()) lines.push_back("");
        }
        else {
            lines.push_back("");
        }
        dirty = false;
    }

    // ── Datei speichern ─────────────────────────────────────────────────────
    bool save() {
        if (filename.empty()) {
            filename = prompt("Dateiname: ");
            if (filename.empty()) { setStatus("Abgebrochen."); return false; }
        }
        std::ofstream f(filename);
        if (!f.is_open()) { setStatus("Fehler beim Speichern!"); return false; }
        for (size_t i = 0; i < lines.size(); i++) {
            f << lines[i];
            if (i + 1 < lines.size()) f << '\n';
        }
        dirty = false;
        setStatus("Gespeichert: " + filename);
        return true;
    }

    // ── Undo ────────────────────────────────────────────────────────────────
    void pushUndo() {
        if (undoStack.size() >= 50) undoStack.erase(undoStack.begin());
        undoStack.push_back({ lines, cx, cy });
    }

    void undo() {
        if (undoStack.empty()) { setStatus("Nichts rückgängig zu machen."); return; }
        auto& e = undoStack.back();
        lines = e.lines; cx = e.cx; cy = e.cy;
        undoStack.pop_back();
        dirty = true;
        setStatus("Rückgängig.");
    }

    // ── Cursor bewegen ──────────────────────────────────────────────────────
    void moveCursor(int key) {
        std::string& row = lines[cy];
        switch (key) {
        case KEY_ARROW_LEFT:
            if (cx > 0) cx--;
            else if (cy > 0) { cy--; cx = (int)lines[cy].size(); }
            break;
        case KEY_ARROW_RIGHT:
            if (cx < (int)row.size()) cx++;
            else if (cy < (int)lines.size() - 1) { cy++; cx = 0; }
            break;
        case KEY_ARROW_UP:
            if (cy > 0) { cy--; cx = std::min(cx, (int)lines[cy].size()); }
            break;
        case KEY_ARROW_DOWN:
            if (cy < (int)lines.size() - 1) { cy++; cx = std::min(cx, (int)lines[cy].size()); }
            break;
        case KEY_HOME:  cx = 0; break;
        case KEY_END:   cx = (int)row.size(); break;
        case KEY_PAGE_UP:
            cy = std::max(0, cy - (screenRows - 2));
            cx = std::min(cx, (int)lines[cy].size());
            break;
        case KEY_PAGE_DOWN:
            cy = std::min((int)lines.size() - 1, cy + (screenRows - 2));
            cx = std::min(cx, (int)lines[cy].size());
            break;
        }
    }

    // ── Zeichen einfügen ────────────────────────────────────────────────────
    void insertChar(char c) {
        pushUndo();
        lines[cy].insert(cx, 1, c);
        cx++;
        dirty = true;
    }

    // ── Neue Zeile ──────────────────────────────────────────────────────────
    void insertNewline() {
        pushUndo();
        std::string rest = lines[cy].substr(cx);
        lines[cy].erase(cx);
        lines.insert(lines.begin() + cy + 1, rest);
        cy++; cx = 0;
        dirty = true;
    }

    // ── Zeichen löschen ─────────────────────────────────────────────────────
    void deleteChar() {
        pushUndo();
        if (cx > 0) {
            lines[cy].erase(cx - 1, 1);
            cx--;
        }
        else if (cy > 0) {
            int newcx = (int)lines[cy - 1].size();
            lines[cy - 1] += lines[cy];
            lines.erase(lines.begin() + cy);
            cy--; cx = newcx;
        }
        dirty = true;
    }

    void deleteCharForward() {
        if (cx < (int)lines[cy].size()) {
            pushUndo();
            lines[cy].erase(cx, 1);
            dirty = true;
        }
        else if (cy < (int)lines.size() - 1) {
            pushUndo();
            lines[cy] += lines[cy + 1];
            lines.erase(lines.begin() + cy + 1);
            dirty = true;
        }
    }

    // ── Suchen ──────────────────────────────────────────────────────────────
    void search() {
        std::string q = prompt("Suchen: ");
        if (q.empty()) return;
        searchQuery = q;
        // Vorwärts suchen ab Cursor+1
        for (int i = cy; i < (int)lines.size(); i++) {
            size_t start = (i == cy) ? cx + 1 : 0;
            size_t pos = lines[i].find(q, start);
            if (pos != std::string::npos) {
                cy = i; cx = (int)pos;
                setStatus("Gefunden.");
                return;
            }
        }
        // Wrap around
        for (int i = 0; i <= cy; i++) {
            size_t pos = lines[i].find(q);
            if (pos != std::string::npos) {
                cy = i; cx = (int)pos;
                setStatus("Gefunden (von vorne).");
                return;
            }
        }
        setStatus("Nicht gefunden: " + q);
    }

    // ── Prompt (Eingabezeile) ────────────────────────────────────────────────
    std::string prompt(const std::string& msg) {
        std::string buf;
        while (true) {
            renderPrompt(msg + buf);
            int k = readKey();
            if (k == KEY_ENTER) break;
            if (k == KEY_ESCAPE) { buf.clear(); break; }
            if (k == KEY_BACKSPACE && !buf.empty()) buf.pop_back();
            else if (k >= 32 && k < 127) buf += (char)k;
        }
        return buf;
    }

    // ── Scroll anpassen ─────────────────────────────────────────────────────
    void scroll() {
        if (cy < rowoff) rowoff = cy;
        if (cy >= rowoff + screenRows - 1) rowoff = cy - screenRows + 2;
        if (cx < coloff) coloff = cx;
        if (cx >= coloff + screenCols) coloff = cx - screenCols + 1;
    }

    // ── Status setzen ───────────────────────────────────────────────────────
    void setStatus(const std::string& msg) { statusMsg = msg; }

    // ─── Rendering ──────────────────────────────────────────────────────────

    void renderPrompt(const std::string& msg) {
        std::string buf;
        // Zur letzten Zeile springen
        buf += "\x1b[" + std::to_string(screenRows) + ";1H";
        buf += "\x1b[7m"; // invertiert
        std::string line = msg.substr(0, screenCols);
        line += std::string(screenCols - (int)line.size(), ' ');
        buf += line;
        buf += "\x1b[m";
        write(STDOUT_FILENO, buf.c_str(), buf.size());
    }

    void render() {
        scroll();
        std::string buf;
        buf.reserve(16384);
        buf += "\x1b[?25l";  // Cursor verstecken
        buf += "\x1b[H";     // Zum Anfang

        // ── Zeilen rendern ────────────────────────────────────────────────
        for (int y = 0; y < screenRows - 1; y++) {
            int fileRow = y + rowoff;
            buf += "\x1b[K"; // Zeile leeren
            if (fileRow < (int)lines.size()) {
                const std::string& l = lines[fileRow];
                int len = std::max(0, (int)l.size() - coloff);
                len = std::min(len, screenCols);
                if (len > 0) buf += l.substr(coloff, len);
            }
            else {
                buf += "\x1b[34m~\x1b[m"; // Tilde für leere Zeilen
            }
            buf += "\r\n";
        }

        // ── Statusleiste ─────────────────────────────────────────────────
        buf += "\x1b[7m"; // invertiert
        std::string left = " " + (filename.empty() ? "[Neue Datei]" : filename);
        if (dirty) left += " [*]";
        std::string right = std::to_string(cy + 1) + ":" + std::to_string(cx + 1) + " ";
        std::string middle = statusMsg;
        statusMsg.clear();

        int pad = screenCols - (int)left.size() - (int)right.size();
        std::string status = left;
        if (pad > 0 && !middle.empty()) {
            int mpad = (pad - (int)middle.size()) / 2 + (int)left.size();
            while ((int)status.size() < mpad) status += ' ';
            status += middle;
        }
        while ((int)status.size() < screenCols - (int)right.size()) status += ' ';
        status += right;
        status = status.substr(0, screenCols);
        buf += status;
        buf += "\x1b[m";

        // ── Hilfezeile ───────────────────────────────────────────────────
        buf += "\r\n\x1b[K";
        buf += "\x1b[90m ^S Speichern  ^Q Beenden  ^F Suchen  ^Z Rückgängig \x1b[m";

        // ── Cursor positionieren ─────────────────────────────────────────
        int screenX = cx - coloff + 1;
        int screenY = cy - rowoff + 1;
        buf += "\x1b[" + std::to_string(screenY) + ";" + std::to_string(screenX) + "H";
        buf += "\x1b[?25h"; // Cursor zeigen

        write(STDOUT_FILENO, buf.c_str(), buf.size());
    }

    // ─── Hauptschleife ───────────────────────────────────────────────────────

    void run() {
        getWindowSize(screenRows, screenCols);
        screenRows -= 2; // Platz für Status + Hilfe

        // Terminal löschen
        write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);

        while (true) {
            render();
            int k = readKey();

            switch (k) {
            case KEY_CTRL_Q:
                if (dirty) {
                    std::string ans = prompt("Ungespeicherte Änderungen! Wirklich beenden? (j/n): ");
                    if (ans != "j" && ans != "J") { setStatus("Abgebrochen."); break; }
                }
                // Terminal aufräumen und beenden
                write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
                exit(0);

            case KEY_CTRL_S:
                save();
                break;

            case KEY_CTRL_F:
                search();
                break;

            case KEY_CTRL_Z:
                undo();
                break;

            case KEY_ENTER:
                insertNewline();
                break;

            case KEY_BACKSPACE:
                deleteChar();
                break;

            case KEY_DEL:
                deleteCharForward();
                break;

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
                if (k >= 32 && k < 127)
                    insertChar((char)k);
                break;
            }
        }
    }
};

// ─── Einstiegspunkt ──────────────────────────────────────────────────────────

int main(int argc, char* argv[]) {
    enableRawMode();
    Editor ed;

    if (argc >= 2) {
        ed.open(argv[1]);
        ed.setStatus("\"" + std::string(argv[1]) + "\" geladen.");
    }
    else {
        ed.setStatus("Neue Datei — Ctrl+S zum Speichern.");
    }

    ed.run();
    return 0;
}
