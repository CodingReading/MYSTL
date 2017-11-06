#include "test.h"
#include "priority_queue_test.h"

namespace mySTL {
    namespace priority_queue_test {
        void test_case1() {
            myPrique<int> prique;
            for (int i = 1; i <= 1000; ++i)
                prique.push(i);
            assert(prique.top() == 1000 && prique.size() == 1000);

            while (!prique.empty()) {
                prique.pop();
            }
            assert(prique.size() == 0);
        }

        void test_case2() {
            int ia[10] = { 1,6,3,7,9,2,5,8,4,10 };
            myPrique<int> prique(ia, ia + 10);
            assert(prique.top() == 10 && prique.size() == 10);
            while (prique.top() != 1) {
                prique.pop();
            }
            assert(prique.size() == 1);
        }
        
        void test_all() {
            test_case1();
            test_case2();
        }
    }
}