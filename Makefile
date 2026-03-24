# Makefile for Shortcuter

dbuild:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build

build:
	echo Building Release
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build
	echo Moving to Release Dir
	mkdir -p rbuild
	cp build/sx rbuild/sx
	cp build/sx-multi rbuild/sx-multi
	cp build/test rbuild/test

run:
	$(MAKE) build
	./build/sx

install:
	echo Sudo required here
	$(MAKE) build
	mv rbuild/sx /usr/local/bin/sx
	mv rbuild/sx-multi /usr/local/bin/sx-multi

clean:
	rm -rf build

.PHONY: dbuild build run install clean buildui

buildui:
	cd SX-UI-Linux && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DSX_BUILD_UI=ON && cmake --build build
	