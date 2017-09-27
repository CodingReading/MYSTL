#ifndef UNINITIALIZED_H_
#define UNINITIALIZED_H_

#include "algorithm.h"
#include "iterator.h"
#include "typetraits.h"
#include "construct.h"
#include <memory>

/*
uninitialized_copy          //区间上构造元素
uninitialized_fill_n        //区间上填充n个元素
uninitialized_fill          //区间first--end上填充元素
*/

namespace mySTL {
    //是POD类型，
    template <class InputIterator, class ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
        InputIterator last, ForwardIterator result, __true_type) {
        memcpy(result, first, (last - first) * sizeof(*first));
        return result + (last - first);
    }

    //不是POD类型，对每个元素构造
    template <class InputIterator, class ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
        InputIterator last, ForwardIterator result, __false_type) {
        for (; first != last; ++first, ++result)
            construct(&(*result), *first);
        return result;
    }

    template <class InputIterator, class ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator first,
        InputIterator last, ForwardIterator result) {
        typedef typename __type_traits<iterator_traits<InputIterator>::value_type>::is_POD_type is_POD;
        return __uninitialized_copy_aux(first, last, result, is_POD());
    }

    /*
    template <class InputIterator, class ForwardIterator, class T>
    inline ForwardIterator __uninitialized_copy(InputIterator first,
    InputIterator last, ForwardIterator result, T*) {
    typedef typename _type_traits<T>::is_POD_type is_POD;
    return __uninitialized_copy_aux(first, last, result, is_POD());
    }*/



    //针对char * ,wchar_t* 特化，直接使用memmove
    inline char *uninitialized_copy(const char *first, const char *last,
        char * result) {
        memmove(result, first, last - first);
        return result + (last - first);
    }

    inline wchar_t *uninitialized_copy(const wchar_t *first, const wchar_t *last,
        wchar_t * result) {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first,
        Size n, const T& x) {
        return __uninitialized_fill_n(first, n, x, value_type(first));
    }

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,
        Size n, const T&x, T*) {
        typedef typename __type_traits<T>::is_POD_type is_POD;
        return __uninitialized_fill_n_aux(first, n, x, is_POD());
    }

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
        Size n, const T& x, __true_type) {
        fill_n(first, n, x);
        return first + n;
    }

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
        Size n, const T& x, __false_type) {
        for (; n > 0; --n, ++first)
            construct(&*(first), x);
        return first;
    }


    template <class ForwardIterator, class T>
    inline void uninitialized_fill(ForwardIterator first,
        ForwardIterator last, const T& x) {
        __uninitialized_fill(first, last, x, value_type(x));
    }

    template <class ForwardIterator, class T>
    inline void __uninitialized_fill(ForwardIterator first,
        ForwardIterator last, const T& x, T*) {
        typedef typename __type_traits<T>::is_POD_type is_POD;
        __uninitialized_fill_aux(first, last, x, is_POD());
    }

    template <class ForwardIterator, class T>
    inline void __uninitialized_fill_aux(ForwardIterator first,
        ForwardIterator last, const T& x, __true_type) {
        fill(first, last, x);
    }

    template <class ForwardIterator, class T>
    inline void __uninitialized_fill_aux(ForwardIterator first,
        ForwardIterator last, const T& x, __false_type) {
        for (; first != last; ++first)
            construct(&(*first), x);
    }
}


#endif
