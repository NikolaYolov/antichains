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
}
