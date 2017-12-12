#ifndef FUNCTIONAL_H_
#define FUNCTIONAL_H_

namespace mySTL {
    //      ·Âº¯Êý
    template <class T>
    struct less {
        bool operator() (const T& a, const T& b) {
            return a < b;
        }
    };

    template <class T>
    struct greater {
        bool operator() (const T& a, const T& b) {
            return a > b;
        }
    };

    template <class T>
    struct identity {
        const T& operator() (const T& x) const {
            return x;
        }
    };
}

#endif