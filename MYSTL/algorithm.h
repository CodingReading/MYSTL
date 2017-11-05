#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include "functional.h"
namespace mySTL {
    template <class T>
    T max(const T& a, const T& b) {
        return (a > b) ? a : b;
    }

    template <class T>
    void swap(T& a, T& b) {
        T t = a;
        a = b;
        b = t;
    }

    template<class OutputIterator, class Size, class T>
    OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
        for (; n > 0; --n, ++first)
            *first = value;
        return first;
    }

    //-------------------------堆算法--------------------------
    //顶元素向下比较，找到合适的位置放入
    template <class RandomAccessIterator, class Compare>
    inline void down(RandomAccessIterator cur, RandomAccessIterator head, RandomAccessIterator last, Compare comp) {
        auto parent_index = cur - head;
        auto child_index = (parent_index << 1) + 1;
        auto len = last - head;
        auto num = *cur;
        while (child_index < len) {
            if (child_index + 1 < len && *(head + child_index + 1) > *(head + child_index))
                ++child_index; //选大的
            if (comp(num, *(head + child_index))) {
                *(head + parent_index) = *(head + child_index);
                parent_index = child_index;
                child_index = (parent_index << 1) + 1;
            }
            else
                break;
        }
        *(head + parent_index) = num;
    }

    //底元素向上比较，找到合适的位置放入
    template <class RandomAccessIterator, class Compare>
    inline void up(RandomAccessIterator cur, RandomAccessIterator head, RandomAccessIterator last, Compare comp) {
        if (cur == head)
            return;
        auto child_index = cur - head;
        auto parent_index = (child_index - 1) / 2;
        auto num = *cur;
        while (child_index > parent_index) {
            if (comp(*(head + parent_index), num)) {
                *(head + child_index) = *(head + parent_index);
                child_index = parent_index;
                parent_index = (child_index - 1) / 2;
            }
            else
                break;
        }
        *(head + child_index) = num;
    }

    template <class RandomAccessIterator>
    inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
        make_heap(first, last, mySTL::less<typename mySTL::iterator_traits<RandomAccessIterator>::value_type>());
    }

    //自底向上重排子树
    template <class RandomAccessIterator, class Compare>
    inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        const auto range = last - first;
        if (range < 2)
            return;
        for (auto cur = first + (range >> 1) - 1; cur >= first; --cur) {
            mySTL::down(cur, first, last, comp);
        }
    }

    template <class RandomAccessIterator>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
        push_heap(first, last, typename mySTL::less<mySTL::iterator_traits<RandomAccessIterator>::value_type>());
    }

    template <class RandomAccessIterator, class Compare>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        if (last - first > 1)
            up(last - 1, first, last, comp);
    }

    template <class RandomAccessIterator>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
        pop_heap(first, last, typename less<mySTL::iterator_traits<RandomAccessIterator>::value_type>());
    }

    //将最大值放在容器尾再调整堆,并不移除
    template <class RandomAccessIterator, class Compare>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        if (last - first < 2)
            return;
        swap(*first, *(last - 1));
        if (last - first <= 2)
            return;
        mySTL::down(first, first, last - 1, comp);
    }
  
    template <class RandomAccessIterator>
    inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
        sort_heap(first, last, typename mySTL::less<mySTL::iterator_traits<RandomAccessIterator>::value_type>());
    }

    //pop_heap即可
    template <class RandomAccessIterator, class Compare>
    inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        while (last - first > 1) {
            pop_heap(first, last--);
        }
    }
} 


#endif