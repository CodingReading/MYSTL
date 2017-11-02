#include "stack_test.h"

namespace mySTL {
    namespace stack_test {
        void test_case1() {
            myStack<int> s;
            for (int i = 1; i <= 100; ++i)
                s.push(i);
            assert(s.size() == 100);
            for (int i = 1; i <= 100; ++i)
                s.pop();
            assert(s.empty());
        }

        void test_case2() {
            myStack<int> s;
            for (int i = 1; i <= 1000; ++i)
                s.push(i);
            assert(s.top() == 1000 && s.size() == 1000);
        }

        void test_all() {
            test_case1();
            test_case2();
        }
    }
}