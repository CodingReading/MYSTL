#ifndef VECTOR_TEST_H_
#define VECTOR_TEST_H_

#include "../vector.h"
#include "test.h"
#include <vector>
#include <cassert>

namespace mySTL {
	
	namespace vector_test {

		template <class T>
		using myVec = mySTL::vector<T>;

		template <class T>
		using stdVec = std::vector<T>;

		void test_case1();
		void test_case2();
		void test_case3();
		void test_case4();
		void test_all();
	}
}

#endif
