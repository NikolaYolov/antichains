#ifndef _ANTICHAIN_H_
#define _ANTICHAIN_H_

#include <algorithm> // next_permutation
#include <bitset>
#include <functional> // std::hash
#include <unordered_set>
#include <vector>

#include "progress_bar.h"

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
	for (pivot = 0; pivot < poset.size() && !poset[pivot]; ++pivot) {
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
vector<Permutation> generate_permutations(size_t dim);

template<size_t dim>
Subset<dim> act_subset(Subset<dim> s, const Permutation& perm) {
	Subset<dim> ret;
	for (size_t i = 0; i < s.size(); ++i) {
		ret[perm[i]] = s[i];
	}
	return ret;
}

// Note that there is not a single if in the entire implementation.
template<size_t dim>
Antichain<dim> act_antichain(Antichain<dim> a, const Permutation& perm) {
	Antichain<dim> ret;
	for (size_t i = 0; i < ret.size(); ++i) {
		size_t image = act_subset<dim>(Subset<dim>(i), perm).to_ulong();
		ret[image] = a[i];
	}
	return ret;
}

template<size_t dim>
unordered_set<Antichain<dim> > get_class(Antichain<dim> achain,
					 const vector<Permutation>& perms) {
	unordered_set<Antichain<dim> > ret;

	for (const Permutation& perm : perms)
		ret.insert(act_antichain<dim>(achain, perm));

	return ret;
}

template<size_t dim>
Antichain<dim> representative(const unordered_set<Antichain<dim> >& class_set) {
	auto it = class_set.begin();
	Antichain<dim> min_found = *(it++);
	for (; it != class_set.end(); ++it) {
		if (hash<Antichain<dim> >()(*it) <         // Not ideal.
		    hash<Antichain<dim> >()(min_found)) {  // Don't how to make
			min_found = *it;                   // it nicer.
		}
	}
	return min_found;
}

// With respect to the group action given in the second argument,
// canonise(a) is isomorphic to a; and if a and b are isomorphic,
// then canonise(a) == canonise(b).
template<size_t dim>
Antichain<dim> canonise(Antichain<dim> a, const vector<Permutation>& perms) {
	return representative<dim>(get_class<dim>(a, perms));
}

template<size_t dim, bool progress=false>
vector<Antichain<dim> > filter_classes(
	unordered_set<Antichain<dim> >& achains) {

	vector<Permutation> perms = generate_permutations(dim);
	vector<Antichain<dim> > output;

	ProgressBar progbar(achains.size());
	while (achains.size() > 0) {
		if (progress) {
			progbar.SetProgress(progbar.Max() - achains.size());
		}
		Antichain<dim> pivot = *achains.begin();
		auto cur_class = get_class<dim>(pivot, perms);
		output.push_back(representative<dim>(cur_class));
		for (Antichain<dim> cur : cur_class) {
			achains.erase(cur);
		}
	}
	if (progress) {
		progbar.SetProgress(progbar.Max());
	}

	return output;
}

template<size_t dim, bool progress=false>
vector<Antichain<dim> > filter_classes(
	const vector<Antichain<dim> >& input) {
	vector<Permutation> perms = generate_permutations(dim);

        unordered_set<Antichain<dim> > achains(input.begin(), input.end());
	return filter_classes<dim, progress>(achains);
}

template<size_t dim>
bool doesnt_contain_any(Subset<dim> s, SetSystem<dim> system) {
	for (size_t i = 0; i < system.size(); ++i) {
		if (system[i] && le<dim>(i, s)) {
			return false;
		}
	}
	return true;
}

template<size_t dim, bool progress=false>
vector<Antichain<dim> > partial_antichains(Poset<dim> poset) {
	Poset<(dim-1)> lower_poset, upper_poset;
	for (size_t i = 0; i < lower_poset.size(); ++i) {
		lower_poset[i] = poset[i];
	}
	for (size_t i = 0; i < upper_poset.size(); ++i) {
		upper_poset[i] = poset[i + lower_poset.size()];
	}

        vector<Antichain<(dim-1)> > lower_classes =
		filter_classes<(dim-1)>(partial_antichains<(dim-1)>(lower_poset));

	vector<Antichain<dim> > result;
	ProgressBar prog(lower_classes.size());
	for (Antichain<dim-1> lower_part : lower_classes) {
		if (progress) {
			prog.RegisterProgress();
		}

		Poset<(dim-1)> not_more_poset;
		for (size_t i = 0; i < upper_poset.size(); ++i) {
			not_more_poset[i] = upper_poset[i] &&
				doesnt_contain_any<(dim-1)>(i, lower_part);
		}

		vector<Antichain<(dim-1)> > upper_parts
			= generate_antichains<dim-1>(not_more_poset);
		for (Antichain<(dim-1)> upper_part : upper_parts) {
			Antichain<dim> add_me;
			for (size_t i = 0; i < lower_part.size(); ++i) {
				add_me[i] = lower_part[i];
			}
			for (size_t i = 0; i < upper_part.size(); ++i) {
				add_me[i + lower_part.size()] = upper_part[i];
			}
			result.push_back(add_me);
		}
	}
	return result;
}

template<>
vector<Antichain<0> > partial_antichains<0, false>(Poset<0>);

template<>
vector<Antichain<0> > partial_antichains<0, true>(Poset<0>);

#endif // _ANTICHAIN_H_
