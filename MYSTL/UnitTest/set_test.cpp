#include "set_test.h"

namespace mySTL {
    namespace set_test {
        void test_case1() {
            int a[10] = { 5,4,3,2,1,5,4,3,6,2 };
            mySet<int> s1(a, a + 10);
            mySet<int> s2(s1);
            mySet<int> s3(s2.begin(), s2.end());
            mySTL::test::container_equal(s1, s2);
            mySTL::test::container_equal(s2, s3);
        }

        void test_case2() {
            mySet<int> s1;
            for (int i = 0; i < 10000; ++i)
                s1.insert(i % 10);
            assert(s1.size() == 10);
            s1.erase(1);
            s1.erase(2);
            for (int i = 0; i < 8; ++i)
                s1.erase(s1.begin());
            assert(s1.size() == 0);
        }

        void test_case3() {
            mySet<int> s1;
            for (int i = 0; i < 10000; ++i)
                s1.insert(i % 100);
            assert(s1.size() == 100);
            s1.erase(s1.begin(), s1.end());
            assert(s1.empty());
        }

        void test_case4() {
            mySet<int> s1;
            for (int i = 0; i < 10000; ++i)
                s1.insert(i % 100);
            auto it = s1.find(50);
            if (it != s1.end())
                assert(*it == 50);
            size_t n = s1.count(99);
            assert(n == 1);
        }

        void test_all() {
            test_case1();
            test_case2();
            test_case3();
            test_case4();
        }
    }
}