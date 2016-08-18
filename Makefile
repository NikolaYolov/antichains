all: antichain optimised test setup_tests

antichain: antichain.cpp antichain.h fileio.cpp fileio.h main.cpp progress_bar.h
	g++ antichain.cpp fileio.cpp main.cpp -o antichain -Wall -std=c++11

optimised: antichain.cpp antichain.h fileio.cpp fileio.h main.cpp progress_bar.h
	g++ antichain.cpp fileio.cpp main.cpp -o optimised -O3 -Wall -std=c++11

setup_tests: antichain.cpp antichain.h fileio.cpp fileio.h setup.cpp progress_bar.h
	g++ antichain.cpp fileio.cpp setup.cpp -o setup_tests -O3 -Wall -std=c++11

test:  antichain.cpp antichain.h fileio.cpp fileio.h test.cpp test.h progress_bar.h
	g++ antichain.cpp fileio.cpp test.cpp -o test -O3 -Wall -std=c++11
