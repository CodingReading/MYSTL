#ifndef HEAP_TEST_H_
#define HEAP_TEST_H_

#include "../algorithm.h"
#include "../vector.h"
#include <cassert>

namespace mySTL {
    namespace heap_test {
        template <class T>
        using myVec = mySTL::vector<T>;

        void test_case1();
        void test_case2();
        void test_all();
    }
}

#endif