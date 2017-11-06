/*
    底层容器使用vector，算法用heap系列算法
*/

#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_


#include "algorithm.h"
#include "functional.h"
#include "vector.h"

namespace mySTL {
    template <class T, class Container = mySTL::vector<T>,
    class Compare = mySTL::less<typename Container::value_type>>
    class priority_queue {
    public:
        typedef typename Container::value_type   value_type;
        typedef typename Container::size_type    size_type;
        typedef typename Container::reference    reference;

    public:
        priority_queue() :c() {}
        explicit priority_queue(const Compare& x) : c(), comp(x) {}

        template <class InputIterator>
        priority_queue(InputIterator first, InputIterator last) :
            c(first, last) {
            make_heap(c.begin(), c.end(), comp);
        }

        template <class InputIterator>
        priority_queue(InputIterator first, InputIterator last, const Compare& x) : c(first, last), comp(x) {
            make_heap(c.begin(), c.end(), comp);
        }

        bool empty() const { return c.empty(); }
        size_type size() { return c.size(); }
        value_type top() { return c.front(); }
        void push(const value_type& val) {
            c.push_back(val);
            push_heap(c.begin(), c.end(), comp);
        }
        void pop() {
            pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }
    private:
        Container c;
        Compare   comp;
    };
}


#endif
