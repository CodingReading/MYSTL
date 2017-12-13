#include "set_test.h"

namespace mySTL {
    namespace set_test {
        void test_case1() {
            int a[5] = { 5,4,3,2,1 };
            mySet<int> s(a, a + 5);
            mySTL::test::container_print(s);
        }


        void test_all() {
            test_case1();
        }
    }
}