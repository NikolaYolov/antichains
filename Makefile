all: antichain

antichain: main.cpp antichain.h
	g++ main.cpp -o antichain -Wall

optimised: main.cpp antichain.h
	g++ main.cpp -o optimised -O3 -Wall
