# Makefile for Shortcuter

build:
	g++ -O3 -march=native -flto tools/sx_main.cpp src/*.cpp -o sx
	g++ -03 -march=native -flto tools/mutil_main.cpp src/*.cpp -o multi

run:
	$(MAKE) build
	./sx

install:
	$(MAKE) build
	mv sx /usr/local/bin/sx
