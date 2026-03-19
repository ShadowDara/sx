# Makefile for Shortcuter

dbuild:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build

build:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build

run:
	$(MAKE) build
	.build/sx

install:
	echo Sudo required here
	$(MAKE) build
	mv build/sx /usr/local/bin/sx
	mv build/sx-multi /usr/local/bin/sx-multi

clean:
	rm -rf build

.PHONY: dbuild build run install clean
