#ifndef DEQUE_TEST_H_
#define DEQUE_TEST_H_

#include "../deque.h"
#include "test.h"
#include <cassert>

namespace mySTL {
    namespace deque_test {
        template<class T>
        using myDeque = mySTL::deque<T>;

        void test_case1();
        void test_case2();
        void test_case3();
        void test_case4();

        void test_all();
    }
}

#endif