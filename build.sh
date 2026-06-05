#!/data/data/com.termux/files/usr/bin/bash

# Ordner auf dem internen Speicher
SRC="/storage/emulated/0/cxxyyx"

# Zielordner in Termux
DST="$HOME/cxxyyx"

# Build-Ordner zurück auf internen Speicher
OUT_BUILD="/storage/emulated/0/cxxyyx_build"

echo "== Alte Dateien löschen =="

# Zielordner komplett leeren
#rm -rf "$DST"

# Alten Output-Build löschen
#rm -rf "$OUT_BUILD"

echo "== Dateien kopieren =="

mkdir -p "$DST"

cp -r "$SRC"/* "$DST"/

cd "$DST" || exit 1

echo "== Build Ordner erstellen =="

mkdir -p build
cd build || exit 1

echo "== CMake konfigurieren =="

cmake ..

if [ $? -ne 0 ]; then
    echo "CMake Fehler!"
    exit 1
fi

echo "== Kompilieren =="

make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "Compile Fehler!"
    exit 1
fi

echo "== Build zurück auf Storage kopieren =="

mkdir -p "$OUT_BUILD"

cp -r ./* "$OUT_BUILD"/

echo "== Ausführen =="

# Erstes ausführbares File suchen
EXEC=$(find . -maxdepth 1 -type f -executable | head -n 1)

if [ -z "$EXEC" ]; then
    echo "Keine executable gefunden!"
    exit 1
fi

"$EXEC"

./jss
