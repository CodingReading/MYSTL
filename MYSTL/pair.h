#ifndef PAIR_H_
#define PAIR_H_

namespace mySTL {
    template <class T1, class T2>
    struct pair {
        T1  first;
        T2  second;

        pair():first(T1()), second(T2()){}
        pair(const T1& x, const T2& y): first(x), second(y){}
    };

    template <class T1, class T2>
    inline bool operator== (const pair<T1, T2>& x, const pair<T1, T2>& y) {
        return x.first == y.first && x.second == y.second;
    }

    template <class T1, class T2>
    inline bool operator!= (const pair<T1, T2>& x, const pair<T1, T2>& y) {
        return !(x == y);
    }

    template <class T1, class T2>
    inline bool operator< (const pair<T1, T2>& x, const pair<T1, T2>& y) {
        return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
    }

    template <class T1, class T2>
    inline bool operator>= (const pair<T1, T2>& x, const pair<T1, T2>& y) {
        return !(x < y);
    }

    template <class T1, class T2>
    inline bool operator> (const pair<T1, T2>& x, const pair<T1, T2>& y) {
        return y < x;
    }

    template <class T1, class T2>
    inline bool operator<= (const pair<T1, T2>& x, const pair<T1, T2>& y) {
        return !(y < x);
    }

    template <class T1, class T2>
    pair<T1, T2> make_pair(T1 first, T2 second) {
        return pair<T1, T2>(first, second);
    }

}



#endif