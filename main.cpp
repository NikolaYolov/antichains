#include <iostream>

#include "antichain.h"
#include "fileio.h"

static const size_t dim = 7;

int main() {
	{
		auto cube = hypercube<dim>();
		auto partial = partial_antichains<dim, true>(cube);
		cout << "Started storing in file...";
		cout.flush();
		write_file<dim>(partial, FileName::AntichainsBinary);
		cout << "done." << endl;
	}
	{
		auto partial = read_file_as_set<dim>(FileName::AntichainsBinary);
		auto classes = filter_classes<dim, true>(partial);
		cout << "Started storing in file...";
		cout.flush();
		write_file<dim>(classes, FileName::ClassesBinary);
		cout << "done." << endl;
	}
}
