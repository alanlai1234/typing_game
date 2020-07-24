all: main

main : main.cpp
	g++ -std=c++11 -lpanel -lncurses main.cpp -o main
