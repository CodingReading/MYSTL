#ifndef CONSTRUCT_H_
#define CONSTRUCT_H_

#include "typetraits.h"
#include <new>

namespace mySTL {
    template <class T1, class T2 >
    inline void construct(T1 *p, const T2& value) {
        //placement new 将初值设定到p所指空间
        new (p) T1(value);	//调用T1::T1(value)
    }

    template <class T>
    inline void destroy(T* pointer) {
        pointer->~T();
    }

    template <class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last) {
        __destroy(first, last, value_type(first));
    }

    template <class ForwardIterator, class T>
    inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
        typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, last, trivial_destructor());
    }

    //如果元素类型没有 trivial destructor
    template <class ForwardIterator>
    inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type) {
        for (; first < last; ++first)
            destroy(&*first);
    }

    //有trivial destructor  什么也不做
    template <class ForwardIterator>
    inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

    //针对基本类型特化
    inline void destroy(char *, char *) {};

    inline void destroy(int *, int *) {};

    inline void destroy(long *, long *) {};
    inline void destroy(long long *, long long *) {};
    inline void destroy(float *, float *) {};
    inline void destroy(double *, double *) {};
}

#endif