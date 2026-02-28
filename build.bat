@echo off

zig c++ -std=c++20 src/main.cpp src/oscore.cpp src/sc.cpp -o sc.exe
.\sc -h
