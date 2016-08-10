#ifndef _ANTICHAIN_H_
#define _ANTICHAIN_H_

#include <algorithm> // next_permutation
#include <bitset>
#include <functional> // std::hash
#include <unordered_set>
#include <vector>

using namespace::std;

template<size_t dim>
using Subset = bitset<dim>;

template<size_t dim>
using SetSystem = bitset<1 << dim>;

template<size_t dim>
using Antichain = SetSystem<dim>;

template<size_t dim>
using Poset = SetSystem<dim>;

template<size_t dim>
bool le(Subset<dim> lhs, Subset<dim> rhs) {
	return lhs == (lhs & rhs);
}

template<size_t dim>
bool comparable(Subset<dim> lhs, Subset<dim> rhs) {
	return le<dim>(lhs, rhs) || le(rhs, lhs);
}

// For external usage use the function below.
template<size_t dim>
void generate_antichains_r(Poset<dim> poset, vector<Antichain<dim> >& output) {
	size_t pivot;
	// Choose the first contained set as pivot.
	for (pivot = 0; pivot < poset.size() && !poset.test(pivot); ++pivot) {
	}

	if (pivot == poset.size()) {
		// If the poset is empty, the only antichain is the empty set.
		output.push_back(Antichain<dim>());
		return;
	}

	poset.reset(pivot); // Clear pivot;

	Poset<dim> incomparable = poset;
	for (size_t i_set = pivot+1; i_set < poset.size(); ++i_set) {
		// Clear the ith bit if i_set and pivot are comparable
		Subset<dim> lhs = Subset<dim>(pivot);
		Subset<dim> rhs = Subset<dim>(i_set);
		incomparable[i_set] = (!comparable<dim>(lhs, rhs) &&
				       incomparable[i_set]);
	}

	// Generate the antichains not containing pivot
	generate_antichains_r<dim>(poset, output);
	size_t i = output.size();
	// Generate the antichains containing pivot
	generate_antichains_r<dim>(incomparable, output);

	// Add pivot to the latter set.
	for(; i < output.size(); ++i) {
		output[i].set(pivot);
	}
}

template<size_t dim>
Poset<dim> hypercube() {
	return Poset<dim>().set();
}

template<size_t dim>
vector<Antichain<dim> > generate_antichains(Poset<dim> poset) {
	vector<Antichain<dim> > ret;
	generate_antichains_r<dim>(poset, ret);
	return ret;
}

typedef vector<size_t> Permutation;

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

template<size_t dim>
Subset<dim> act(Subset<dim> s, const Permutation& perm) {
	Subset<dim> ret;
	for (size_t i = 0; i < s.size(); ++i) {
		ret[perm[i]] = s.test(i);
	}
	return ret;
}

// Note that there is not a single if in the entire implementation.
template<size_t dim>
Antichain<dim> act(Antichain<dim> a, const Permutation& perm) {
	Antichain<dim> ret;
	for (size_t i = 0; i < ret.size(); ++i) {
		size_t image = act<dim>(Subset<dim>(i), perm).to_ulong();
		ret[image] = a.test(i);
	}
	return ret;
}

// With respect to the group action given in the second argument,
// canonise(a) is isomorphic to a; and if a and b are isomorphic,
// then canonise(a) == canonise(b).
template<size_t dim>
Antichain<dim> canonise(Antichain<dim> a, const vector<Permutation>& perms) {
	Antichain<dim> min_found = a;
	for (const Permutation& perm : perms) {
		Antichain<dim> test = act<dim>(a, perm);
		if (hash<Antichain<dim> >()(test) <        // Not ideal.
		    hash<Antichain<dim> >()(min_found)) {  // Don't how to make
			min_found = test;                  // it nicer.
		}
	}
	return min_found;
}

template<size_t dim>
unordered_set<Antichain<dim> > generate_classes(
	const vector<Antichain<dim> >& input) {
	vector<Permutation> perms = generate_permutations(dim);

	unordered_set<Antichain<dim> > output;
	for (Antichain<dim> a : input) {
		output.insert(canonise<dim>(a, perms));
	}
	return output;
}

#endif // _ANTICHAIN_H_
