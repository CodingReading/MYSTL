#ifndef LIST_TEST_H_
#define LIST_TEST_H_

#include "../list.h"
#include "test.h"
#include <cassert>

namespace mySTL {
	namespace list_test {
		template <class T>
		using myList = mySTL::list<T>;

		void test_case1();
		void test_case2();
		void test_case3();
		void test_case4();
		void test_case5();
		void test_all();
	}
}




#endif