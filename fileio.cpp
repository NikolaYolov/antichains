#include "fileio.h"

string convertFileName(FileName fn, size_t dim) {
	string header;
	switch (fn) {
	case FileName::AntichainsText:
	case FileName::AntichainsBinary:
		header = "data/antichains";
		break;
	case FileName::AntichainsBackupText:
	case FileName::AntichainsBackupBinary:
		header = "data/antichainsbackup";
		break;
	case FileName::ClassesText:
	case FileName::ClassesBinary:
		header = "data/classes";
		break;
	case FileName::ClassesBackupText:
	case FileName::ClassesBackupBinary:
		header = "data/classesbackup";
		break;
	default:
		assert(false);
	};

	string suffix;
	if (is_binary(fn)) {
		suffix = ".bin";
	} else {
		suffix = ".txt";
	}

	return header + to_string(dim) + suffix;
}

bool is_binary(FileName fn) {
	switch (fn) {
	case FileName::AntichainsText:
	case FileName::AntichainsBackupText:
	case FileName::ClassesText:
	case FileName::ClassesBackupText:
		return false;
		break;
	case FileName::AntichainsBinary:
	case FileName::AntichainsBackupBinary:
	case FileName::ClassesBinary:
	case FileName::ClassesBackupBinary:
		return true;
	default:
		assert(false);
	};
}
