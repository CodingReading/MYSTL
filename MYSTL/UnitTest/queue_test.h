#ifndef QUEUE_TEST_H_
#define QUEUE_TEST_H_

#include "../queue.h"
#include <cassert>

namespace mySTL {
    namespace queue_test {
        template <class T>
        using myQueue = mySTL::queue<T>;

        void test_case1();
        void test_case2();
        void test_all();
    }
}

#endif