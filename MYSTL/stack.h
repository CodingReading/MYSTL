#ifndef STACK_H_
#define STACK_H_

#include "deque.h"

namespace mySTL {
    template <class T, class Container = mySTL::deque<T>>
    class stack {
    public:
        typedef typename Container::value_type   value_type;
        typedef typename Container::reference    reference;
        typedef typename Container::size_type    size_type;
    public:
        explicit stack(Container &t = Container()):c(t){}
        bool empty() const { return c.empty(); }
        size_type size() { return c.size(); }
        void push(value_type& val) { c.push_back(val); }
        void pop() { c.pop_back(); }
        value_type& top() { return c.back(); }
    private:
        Container c;
    };
}

#endif // !STACK_H_
