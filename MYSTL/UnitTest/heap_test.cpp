#include "heap_test.h"
#include "test.h"

namespace mySTL {
    namespace heap_test {
        void test_case1() {
            myVec<int> v;
            for (int i = 1; i <= 1000; ++i)
                v.push_back(i);
            make_heap(v.begin(), v.end());
            assert(v.front() == 1000 && v.size() == 1000);

            v.push_back(10000);
            push_heap(v.begin(), v.end());
            assert(v.front() == 10000);
            pop_heap(v.begin(), v.end());
            assert(v.back() == 10000);
        }

        void test_case2() {
            myVec<int> v;
            for (int i = 1000; i > 0; --i)
                v.push_back(i);
            make_heap(v.begin(), v.end());
            sort_heap(v.begin(), v.end());
            assert(v[0] == 1 && v[500] == 501 && v[999] == 1000);

            myVec<int> v1;
            make_heap(v1.begin(), v1.end());
            push_heap(v1.begin(), v1.end());
            pop_heap(v1.begin(), v1.end());
            make_heap(v1.begin(), v1.end());
            sort_heap(v1.begin(), v1.end());

            v1.push_back(1);
            make_heap(v1.begin(), v1.end());
            push_heap(v1.begin(), v1.end());
            pop_heap(v1.begin(), v1.end());
            make_heap(v1.begin(), v1.end());
            sort_heap(v1.begin(), v1.end());

            v1.push_back(2);
            make_heap(v1.begin(), v1.end());
            push_heap(v1.begin(), v1.end());
            pop_heap(v1.begin(), v1.end());
            make_heap(v1.begin(), v1.end());
            sort_heap(v1.begin(), v1.end());            
            assert(v1.front() == 1 && v1.back() == 2);
        }

        void test_all() {
            test_case1();
            test_case2();
        }

    }
}