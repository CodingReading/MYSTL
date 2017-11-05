#ifndef FUNCTIONAL_H_
#define FUNCTIONAL_H_

namespace mySTL {
    template <class T>
    struct less {
        bool operator() (const T& a, const T& b) {
            return a < b;
        }
    };
}

#endif