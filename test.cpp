#include "test.h"

int main() {
	computation_test<0>();
	computation_test<1>();
	computation_test<2>();
	computation_test<3>();
	computation_test<4>();
	computation_test<5>();
	computation_test<6>();

 	consistency_test<0>();
 	consistency_test<1>();
	consistency_test<2>();
        consistency_test<3>();
        consistency_test<4>();
        consistency_test<5>();
        consistency_test<6>();

	phase_test<0>(1);
	phase_test<1>(1);
	phase_test<2>(1);
	phase_test<3>(1);
	phase_test<4>(1);
	phase_test<5>(10);
	phase_test<6>(1000);
}
