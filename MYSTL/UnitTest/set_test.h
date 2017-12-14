#ifndef SET_TEST_H_
#define SET_TEST_H_

#include "../set.h"
#include "test.h"
#include <cassert>

namespace mySTL {
    namespace set_test {
        template <class T>
        using mySet = mySTL::set<T>;

        void test_case1();
        void test_case2();
        void test_case3();
        void test_case4();
        void test_all();
    }
}

#endif // !SET_TEST_H_
