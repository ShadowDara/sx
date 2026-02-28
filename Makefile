# Makefile for Shortcuter

build:
	g++ -O3 -march=native -flto src/*.cpp -o sx

run:
	$(MAKE) build
	./sx

install:
	$(MAKE) build
	mv sx /usr/local/bin/sx
