#include "deque_test.h"

namespace mySTL {
    namespace deque_test {
        void test_case1() {
            myDeque<int> deq1(2000, 8);
            myDeque<int> deq2(deq1);
            mySTL::test::container_equal(deq1, deq2);
            myDeque<int> deq3(deq2.begin(), deq2.end());
            mySTL::test::container_equal(deq2, deq3);
            myDeque<int> deq4 = deq3;
            mySTL::test::container_equal(deq1, deq4);
            
            myDeque<int> deq5(1000, 8);
            deq5 = deq1;
            mySTL::test::container_equal(deq3, deq5);
        }
        
        void test_case2() {
            myDeque<int> deq1(2000, 8);
            for (int i = 0; i < 10; ++i)
                deq1.push_back(i);
            for (int i = 0; i < 10; ++i)
                deq1.push_front(i);
            for (int i = 0; i < 10; ++i)
                deq1.pop_back();
            for (int i = 0; i < 10; ++i)
                deq1.pop_front();
            assert(deq1.size() == 2000);
        }

        void test_case3() {
            myDeque<int> deq1(10, 8);
            deq1.insert(deq1.begin(), 90, 6);
            deq1.insert(deq1.end(), 100, 8);
            assert(deq1.size() == 200);

            myDeque<int> deq2(deq1.begin(), deq1.end());
            mySTL::test::container_equal(deq1, deq2);
        }

        void test_case4() {
            myDeque<int> deq1(100, 8);
            for (int i = 0; i < 10; ++i) {
                deq1.erase(deq1.begin());
                deq1.erase(deq1.end() - 1);
            }
            assert(deq1.size() == 80);

            myDeque<int> deq2(deq1);
            deq2.erase(deq2.begin(), deq2.begin() + 30);
            assert(deq2.size() == 50);

            deq2.swap(deq1);
            assert(deq1.size() == 50);
            assert(deq2.size() == 80);
            deq2.clear();
            assert(deq2.empty());
        }

        void test_all() {
            test_case1();
            test_case2();
            test_case3();
            test_case4();
        }
    }
}