#ifndef _PHASES_H_
#define _PHASES_H_

#include <iostream>

#include "antichain.h"
#include "fileio.h"

template<size_t dim, bool output=true>
void phase1() {
	if (output)
		cout << "Started phase 1." << endl;
	auto cube = hypercube<dim>();
	auto partial = partial_antichains<dim, output>(cube);
	if (output)
		cout << "Started storing in file...";
	cout.flush();
	write_file<dim>(partial, FileName::AntichainsBinary);
	if (output)
		cout << "done." << endl;
}

template<size_t dim, bool output=true>
void phase2(size_t chunk_size) {
	if (output)
		cout << "Started phase 2." << endl;
	unordered_set<Antichain<dim> > result;
	for (size_t head = 0; ; head += chunk_size) {
		if (output)
			cout << "Processing chunk "
			     << head << " " << head + chunk_size << endl;

		unordered_set<Antichain<dim> > partial = read_file_as_set<dim>(
			FileName::AntichainsBinary,
			head, head+chunk_size);
		if (partial.empty()) {
			break;
		}

		vector<Antichain<dim> > classes = filter_classes<dim, output>(partial);
		result.insert(classes.begin(), classes.end());
	}
	write_file<dim>(result, FileName::ClassesBinary);
}

#endif // _PHASES_H_
