#include "list_test.h"

namespace mySTL {
    namespace list_test {
        void test_case1() {
            myList<int> l1;
            for (int i = 1; i < 11; i++)
                l1.insert(l1.begin(), i);
            assert(l1.size() == 10);
            assert(l1.front() == 10);
            assert(l1.back() == 1);

            for (int i = 0; i < 5; i++) {
                l1.pop_back();
                l1.pop_front();
            }
            assert(l1.empty());
        }

        void test_case2() {
            myList<int> l1;
            //跟踪到chunk_alloc时分配出错，返回的节点为上一个节点
            //当两次相加等于　nobjs崩溃    已修复
            for (int i = 0; i < 10; i++) {
                l1.push_back(i);
            }

            myList<int> l2(l1);
            assert(mySTL::test::container_equal(l1, l2));
            for (int i = 0; i < 5; i++)
                l2.erase(--l2.end());
            assert(l2.size() == 5);

            l2.splice(l2.end(), l1);
            assert(l1.size() == 0);
            assert(l2.size() == 15);

            l2.clear();
            assert(l2.empty());
        }

        void test_case3() {
            myList<int> l1;
            for (int i = 0; i < 10; i++)
                l1.insert(l1.end(), i);
            myList<int> l2;
            for (int i = 0; i < 10; i++)
                l2.insert(l2.begin(), i);
            l2.reverse();
            assert(mySTL::test::container_equal(l1, l2));
        }

        void test_case4() {
            myList<int> l1;
            for (int i = 0; i < 10; i++)
                l1.insert(l1.end(), i);
            myList<int> l2;
            for (int i = 0; i < 10; i++)
                l2.insert(l2.end(), i);
            l1.merge(l2);
            assert(l1.size() == 20);
            assert(l2.empty());
        }

        void test_case5() {
            myList<int> l1;
            for (int i = 0; i < 10; i++)
                l1.insert(l1.begin(), i);
            l1.sort();
            myList<int> l2;
            for (int i = 0; i < 10; i++)
                l2.insert(l2.end(), i);
            assert(mySTL::test::container_equal(l1, l2));
            l1.swap(l2);
        }

        void test_all() {
            test_case1();
            test_case2();
            test_case3();
            test_case4();
            test_case5();
        }
    }
}