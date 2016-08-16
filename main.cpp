#include "antichain.h"
#include "fileio.h"

static const size_t dim = 7;

int main() {
	Poset<dim> cube = hypercube<dim>();
	auto partial = partial_antichains<dim>(cube);
	write_file<dim>(partial, FileName::AntichainsBinary);

	auto compressed = filter_classes<dim>(partial);
	write_file<dim>(partial, FileName::ClassesBinary);
}
