#include "antichain.h"
#include "fileio.h"

template<size_t dim>
void setup_backup() {
	Poset<dim> cube = hypercube<dim>();

	auto full = generate_antichains<dim>(cube);
	write_file<dim>(full, FileName::AntichainsBackupText);
	write_file<dim>(full, FileName::AntichainsBackupBinary);

	auto compressed = filter_classes<dim>(full);
	write_file<dim>(compressed, FileName::ClassesBackupText);
	write_file<dim>(compressed, FileName::ClassesBackupBinary);
}

int main() {
	setup_backup<0>();
	setup_backup<1>();
	setup_backup<2>();
	setup_backup<3>();
	setup_backup<4>();
	setup_backup<5>();
	setup_backup<6>();
}
