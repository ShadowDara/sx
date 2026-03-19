# Makefile for Shortcuter

dbuild:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build

build:
	echo Building Release
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build
	echo Moving to Release Dir
	cp -r /build / rbuild

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

.PHONY: dbuild build run install clean
