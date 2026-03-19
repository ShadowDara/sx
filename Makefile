# Makefile for Shortcuter

dbuild:
	g++ -O0 -g -std=c++20 -march=native -flto tools/sx_main.cpp src/*.cpp -o sx
	g++ -O0 -g -std=c++20 -march=native -flto tools/sx-multi_main.cpp src/*.cpp -o multi
	g++ -O0 -g -std=c++20 -march=native -flto tools/test.cpp src/*.cpp -o test

build:
	g++ -O3 -std=c++20 -march=native -flto tools/sx_main.cpp src/*.cpp -o sx
	g++ -O3 -std=c++20 -march=native -flto tools/sx-multi_main.cpp src/*.cpp -o multi
	g++ -O3 -std=c++20 -march=native -flto tools/test.cpp src/*.cpp -o test

run:
	$(MAKE) build
	./sx

install:
	$(MAKE) build
	mv sx /usr/local/bin/sx
