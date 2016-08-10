#include <iostream>
#include <fstream>

#include "antichain.h"

static const size_t dim = 3;

int main() {
	Poset<dim> cube = hypercube<dim>();
	vector<Antichain<dim> > achains = generate_antichains<dim>(cube);
	cout << "All antichains: " << achains.size() << endl;
	{
		string fname = "antichains" + to_string(dim) + ".txt";
		ofstream outfile(fname);
		outfile << dim << " " << achains.size() << endl;
		for (Antichain<dim> achain : achains) {
			outfile << achain << endl;
		}
	}

	unordered_set<Antichain<dim> > classes = generate_classes<dim>(achains);
	cout << "All classes: " << classes.size() << endl;
	{
		string fname = "classes" + to_string(dim) + ".txt";
		ofstream outfile(fname);
		outfile << dim << " " << classes.size() << endl;
		for (Antichain<dim> achain : classes) {
			outfile << achain << endl;
		}
	}
}
