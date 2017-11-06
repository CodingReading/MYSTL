#ifndef PRIORITY_QUEUE_TEST_H_
#define PRIORITY_QUEUE_TEST_H_

#include "../priority_queue.h"
#include <cassert>

namespace mySTL {
    namespace priority_queue_test {
        template <class T>
        using myPrique = mySTL::priority_queue<T>;

        void test_case1();
        void test_case2();

        void test_all();
    }
}

#endif