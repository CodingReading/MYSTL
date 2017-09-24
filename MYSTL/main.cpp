#include "UnitTest\typetraits_test.h"
#include "UnitTest\vector_test.h"

int main() {
	mySTL::typetraits_test::test_all();
	mySTL::vector_test::test_all();

	return 0;
}