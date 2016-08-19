#ifndef _TEST_H_
#define _TEST_H_

#include <cassert>
#include <vector>
#include <unordered_set>

#include "antichain.h"
#include "fileio.h"
#include "phases.h"

// Checks if achain is an actual antichain: every pair of elements are
// incomparable.
template<size_t dim>
void validate_antichain(Antichain<dim> achain) {
	vector<Subset<dim> > all_sets;

	for (size_t i = 0; i < achain.size(); ++i) {
		if (achain.test(i)) {
			all_sets.push_back(Subset<dim>(i));
		}
	}

	for (size_t i = 0; i + 1 < all_sets.size(); ++i) {
		for (size_t j = i+1; j < all_sets.size(); ++j) {
			if (comparable(all_sets[i], all_sets[j])) {
				assert(false);
			}
		}
	}
}

// Checks if representatives is a set of representatives of different classes,
// and that their union is the first argument (antichains).
template<size_t dim>
void validate_representatives(
	const unordered_set<Antichain<dim> >& antichains,
	const unordered_set<Antichain<dim> >& representatives) {
	unordered_set<Antichain<dim> > rep_union;
	vector<Permutation> perms = generate_permutations(dim);

	for (Antichain<dim> achain : representatives) {
		assert(achain == canonise<dim>(achain, perms));
		auto cur_class = get_class<dim>(achain, perms);
		size_t pre_insert = rep_union.size();
		rep_union.insert(cur_class.begin(), cur_class.end());
		assert(rep_union.size() - pre_insert == cur_class.size());
	}
	assert(rep_union == antichains);
}

template<size_t dim>
void test_pair_sets(const unordered_set<Antichain<dim> >& antichains,
		    const unordered_set<Antichain<dim> >& representatives) {
	cout << "testing " << antichains.size() << " antichains and "
	     << representatives.size() << " classes" << endl;
	for (Antichain<dim> achain : antichains)
		validate_antichain<dim>(achain);

	for (Antichain<dim> achain : representatives)
		validate_antichain<dim>(achain);

	validate_representatives<dim>(antichains, representatives);
}

template<size_t dim>
void consistency_test() {
	cout << "consistency_test<" << dim << ">" << endl;
	test_pair_sets<dim>(read_file_as_set<dim>(FileName::AntichainsText),
			    read_file_as_set<dim>(FileName::ClassesText));
}

// This test computes the sets from scratch and tests against the backup.
// It also saves them to check if the IO works.
template<size_t dim>
void computation_test() {
	cout << "computation_test<" << dim << ">" << endl;
	{
		Poset<dim> cube = hypercube<dim>();
		vector<Antichain<dim> > achains
			= generate_antichains<dim>(cube);
		write_file<dim>(achains, FileName::AntichainsText);
		write_file<dim>(achains, FileName::AntichainsBinary);

	        vector<Antichain<dim> > classes
			= filter_classes<dim, true>(partial_antichains<dim, true>(cube));
		write_file<dim>(classes, FileName::ClassesText);
		write_file<dim>(classes, FileName::ClassesBinary);
	}

	{
		vector<Antichain<dim> > achains_new_text
			= read_file<dim>(FileName::AntichainsText);
		vector<Antichain<dim> > achains_new_bin
			= read_file<dim>(FileName::AntichainsBinary);
		vector<Antichain<dim> > achains_backup_text
			= read_file<dim>(FileName::AntichainsBackupText);
		vector<Antichain<dim> > achains_backup_bin
			= read_file<dim>(FileName::AntichainsBackupBinary);
		assert(achains_new_text == achains_new_bin);
		assert(achains_new_text == achains_backup_text);
		assert(achains_new_text == achains_backup_bin);
	}

	{
	        unordered_set<Antichain<dim> > classes_new_text
			= read_file_as_set<dim>(FileName::ClassesText);
	        unordered_set<Antichain<dim> > classes_new_bin
			= read_file_as_set<dim>(FileName::ClassesBinary);
		unordered_set<Antichain<dim> > classes_backup_text
			= read_file_as_set<dim>(FileName::ClassesBackupText);
		unordered_set<Antichain<dim> > classes_backup_bin
			= read_file_as_set<dim>(FileName::ClassesBackupBinary);
		assert(classes_new_text == classes_new_bin);
		assert(classes_new_text == classes_backup_text);
		assert(classes_new_text == classes_backup_bin);
	}
}

template<size_t dim>
void phase_test(size_t chunk) {
	cout << "Testing phases<" << dim << ">" << endl;
	phase1<dim, false>();
	phase2<dim, false>(chunk);
	unordered_set<Antichain<dim> > classes_new_bin
		= read_file_as_set<dim>(FileName::ClassesBinary);
	unordered_set<Antichain<dim> > classes_backup_bin
		= read_file_as_set<dim>(FileName::ClassesBackupBinary);
	assert(classes_new_bin == classes_backup_bin);
}
#endif // _TEST_H_
