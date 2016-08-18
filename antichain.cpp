#include "antichain.h"

vector<Permutation> generate_permutations(size_t dim) {
	vector<Permutation> output;
	Permutation perm;
	// The loop intialises perm as the first permutation.
	for (size_t i = 0; i < dim; ++i) {
		perm.push_back(i);
	}
	output.push_back(perm);

	while (next_permutation(perm.begin(), perm.end())) {
		output.push_back(perm);
	}

	return output;
}

template<>
vector<Antichain<0> > partial_antichains<0, false>(Poset<0> poset) {
	return generate_antichains<0>(poset);
}

template<>
vector<Antichain<0> > partial_antichains<0, true>(Poset<0> poset) {
	ProgressBar progbar(1);
	progbar.RegisterProgress();
	return generate_antichains<0>(poset);
}
