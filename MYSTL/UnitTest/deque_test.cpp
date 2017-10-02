#include "deque_test.h"

namespace mySTL {
    namespace deque_test {
        void test_case1() {
            deque_iterator<int> it;
            myDeque<int> deq(10, 8);
            mySTL::test::container_print(deq);
        }

        void test_all() {
            test_case1();
        }
    }
}