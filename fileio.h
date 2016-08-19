#ifndef _FILEIO_H_
#define _FILEIO_H_

#include <cassert>
#include <climits> // CHAR_BIT
#include <fstream>
#include <unordered_set>

#include "antichain.h"

using namespace::std;

// Header
enum class FileName {
	AntichainsText,
	AntichainsBackupText,
	ClassesText,
	ClassesBackupText,
	AntichainsBinary,
	AntichainsBackupBinary,
	ClassesBinary,
	ClassesBackupBinary,
};

template<size_t dim>
void write_file(const vector<Antichain<dim> >& , FileName);

template<size_t dim>
void write_file(const unordered_set<Antichain<dim> >& s, FileName fn) {
	vector<Antichain<dim> > v(s.begin(), s.end());
	write_file<dim>(v, fn);
}

template<size_t dim>
vector<Antichain<dim> > read_file(FileName fn, size_t first= 0,
				  size_t last = size_t(-1)) {
	vector<Antichain<dim> > result;
	auto callback = [first, last, &result](Antichain<dim> a, size_t i) {
		if (first <= i && i < last) {
			result.push_back(a);
		}
	};
	read_file<dim, vector<Antichain<dim>>>(fn, result, callback);
	return result;
}

template<size_t dim>
unordered_set<Antichain<dim> > read_file_as_set(FileName fn, size_t first= 0,
				  size_t last = size_t(-1)) {
	unordered_set<Antichain<dim> > result;
	auto callback = [first, last, &result](Antichain<dim> a, size_t i){
		if (first <= i && i < last) {
			result.insert(a);
		}
	};
	read_file<dim, unordered_set<Antichain<dim>>>(fn, result, callback);
	return result;
}

// Implementation
string convertFileName(FileName, size_t);
bool is_binary(FileName);
typedef unsigned char Block;

template<size_t dim>
Antichain<dim> read_antichain_from_binary(ifstream& in_file) {
	Antichain<dim> ret;
	static const size_t block_size = (sizeof Block()) * CHAR_BIT;
	static const size_t num_blocks
		= ret.size() / block_size + bool(ret.size() % block_size);

	for (size_t block_idx = 0; block_idx < num_blocks; ++block_idx) {
		ret <<= block_size;
		Block block;
		in_file.read((char*)&block, sizeof block);
		Antichain<dim> block_bits = block;
		ret |= block_bits;
	}
	return ret;
}

// Reads the least significant (sizeof Block) * CHAR_BIT bits from a
// bitset and returns them as a Block.
template<size_t n>
Block last_block(std::bitset<n> bits) {
	Block ret = 0;
	for (size_t i = 0; i < sizeof ret * CHAR_BIT; ++i) {
		ret |= (bits[i] << i);
	}
	return ret;
}

template<size_t dim>
void write_antichain_to_binary(Antichain<dim> achain, ofstream& out_file) {
	static const size_t block_size = (sizeof Block()) * CHAR_BIT;
	static const size_t num_blocks
		= achain.size() / block_size + bool(achain.size() % block_size);

	for (size_t block_idx = 0; block_idx < num_blocks; ++block_idx) {
		Block block = last_block<achain.size()>(
			achain >> (num_blocks - block_idx - 1) * block_size);
		out_file.write((char*)&block, sizeof block);
	}
}

template<size_t dim>
void write_file(const vector<Antichain<dim> >& achains, FileName fn) {
	if (is_binary(fn)) {
		ofstream outfile(convertFileName(fn, dim), ios::binary);
		size_t dim_var = dim, size_var = achains.size();
		outfile.write((char*)&dim_var, sizeof dim_var);
		outfile.write((char*)&size_var, sizeof size_var);
		for (Antichain<dim> a: achains) {
			write_antichain_to_binary<dim>(a, outfile);
		}
	} else {
		ofstream outfile(convertFileName(fn, dim));
		outfile << dim << " " << achains.size() << endl;
		for (Antichain<dim> a: achains) {
			outfile << a << endl;
		}
	}
}

template<size_t dim, class Container>
	void read_file(FileName fn, Container container,
		       function<void(Antichain<dim>, size_t)> insert) {
	size_t dim_var, size_var;
	if (is_binary(fn)) {
		ifstream infile(convertFileName(fn, dim), ios::binary);
		infile.read((char*)&dim_var, sizeof dim_var);
		infile.read((char*)&size_var, sizeof size_var);
		assert(dim_var == dim);
		for (size_t i = 0; i < size_var; ++i) {
		        insert(read_antichain_from_binary<dim>(infile), i);
		}
	} else {
		ifstream infile(convertFileName(fn, dim));
		infile >> dim_var >> size_var;
		assert(dim_var == dim);
		for (size_t i = 0; i < size_var; ++i) {
			Antichain<dim> a;
			infile >> a;
			insert(a, i);
		}
	}
}

#endif // _FILEIO_H_
