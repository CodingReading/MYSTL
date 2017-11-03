#include "test.h"
#include "queue_test.h"

namespace mySTL {
    namespace queue_test {
        void test_case1() {
            myQueue<int> q;
            for (int i = 0; i < 10000; ++i) {
                q.push(i);
            }

            assert(q.back() == 9999 && q.front() == 0);
            assert(q.size() == 10000);
            for (int i = 0; i < 5000; ++i)
                q.pop();
            assert(q.size() == 5000);
        }

        void test_case2() {
            myQueue<int> q;
            for (int i = 0; i < 1000; ++i) {
                q.push(i);
                q.pop();
            }
            assert(q.empty());
        }

        void test_all() {
            test_case1();
            test_case2();
        }
    }
}

