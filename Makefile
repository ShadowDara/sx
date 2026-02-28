# Makefile for Shortcuter

build:
	g++ -O2 -march=native -flto src/*.cpp -o main

run:
	$(MAKE) build
	./main

install:
	$(MAKE) build
	mv main /usr/local/bin/sx
