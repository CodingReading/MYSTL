#ifndef STACK_TEST_H_
#define STACK_TEST_H_
#include "../stack.h"
#include "test.h"
#include <cassert>

namespace mySTL {
    namespace stack_test {
        template <class T>
        using myStack = mySTL::stack<T>;

        void test_case1();
        void test_case2();
        void test_all();
    }
}

#endif