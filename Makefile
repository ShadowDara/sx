# Makefile for Shortcuter

dbuild:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build

build:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build

run:
	$(MAKE) build
	./sx

install:
	$(MAKE) build
	mv sx /usr/local/bin/sx
