#ifndef QUEUE_H_
#define QUEUE_H_

#include "deque.h"

namespace mySTL {
    template <class T, class Container = deque<T>>
    class queue {
    public:
        typedef typename Container::value_type  value_type;
        typedef typename Container::reference   reference;
        typedef typename Container::size_type   size_type;
    public:
        explicit queue(Container& t = Container()): c(t){}
        bool empty() const { return c.empty(); }
        size_type size() { return c.size(); }
        reference front() { return c.front(); }
        reference back() { return c.back(); }
        void pop() { c.pop_front(); }
        void push(value_type& val) { c.push_back(val); }
    private:
        Container c;
    };
}


#endif

