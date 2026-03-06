#!/bin/bash

# Build with ZIG!

echo Debug Build
zig c++ -g -std=c++20 -O0 tools/sx_main.cpp src/oscore.cpp src/sx.cpp -o sx.exe
zig c++ -g -std=c++20 -O0 tools/multi_main.cpp -o multi.exe

echo Release Build
# For a Release Build with Zig, but this would make the
# Compilation Time Longer!
zig c++ -std=c++20 -O3 tools/sx_main.cpp src/oscore.cpp src/sx.cpp -o sx_release.exe
