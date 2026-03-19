#!/bin/bash

# Build with ZIG!

echo Debug Build
zig c++ -g -std=c++20 -O0 tools/sx_main.cpp src/sx_oscore.cpp src/sx.cpp -o sx.exe
zig c++ -g -std=c++20 -O0 tools/sx-multi_main.cpp src/sx-multi.cpp sx_lib/sx_lib.cpp src/sx_oscore.cpp -o sx-multi.exe
zig c++ -g -std=c++20 -O0 tools/text.cpp src/sx_oscore.cpp src/sx.cpp -o text.exe

echo Release Build
# For a Release Build with Zig, but this would make the
# Compilation Time Longer!
zig c++ -std=c++20 -O3 tools/sx_main.cpp src/sx_oscore.cpp src/sx.cpp -o sx_release.exe
zig c++ -std=c++20 -O3 tools/sx-multi_main.cpp src/sx-multi.cpp sx_lib/sx_lib.cpp src/sx_oscore.cpp -o sx-multi_release.exe
zig c++ -g -std=c++20 -O0 tools/text.cpp src/sx_oscore.cpp src/sx.cpp -o text.exe
