all: antichain optimised test setup_tests

antichain: antichain.cpp antichain.h fileio.cpp fileio.h main.cpp
	g++ antichain.cpp fileio.cpp main.cpp -o antichain -Wall

optimised: antichain.cpp antichain.h fileio.cpp fileio.h main.cpp
	g++ antichain.cpp fileio.cpp main.cpp -o optimised -O3 -Wall

setup_tests: antichain.cpp antichain.h fileio.cpp fileio.h setup.cpp
	g++ antichain.cpp fileio.cpp setup.cpp -o setup_tests -O3 -Wall

test:  antichain.cpp antichain.h fileio.cpp fileio.h test.cpp test.h
	g++ antichain.cpp fileio.cpp test.cpp -o test -Wall -O3
