#include <iostream>

#include "antichain.h"
#include "fileio.h"
#include "phases.h"

static const size_t n = 7;

int main() {
//	phase1<n, true>();
	phase2<n, true>(size_t(1) << 31);
}
