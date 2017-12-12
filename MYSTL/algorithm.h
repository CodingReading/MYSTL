#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include "functional.h"
#include "pair.h"
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
            pop_heap(first, last--, comp);
        }
    }


    //--------------------lower_bound  upper_bound  equal_range-----------------------

    //指向第一个不小于value的元素
    template <class T, class ForwardIterator>
    inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
        const T& value) {
        return __lower_bound(first, last, value, iterator_category(first));
    }

    template <class T, class ForwardIterator>
    ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
        const T& value, forward_iterator_tag) {
        size_t len = 0;     //区间长度
        ForwardIterator t = first;
        while (t != last) {
            ++len;
            ++t;
        }
        ForwardIterator mid;
        while (len > 0) {
            size_t half = len >> 1;
            mid = first;
            for (size_t i = 0; i < half; ++i)
                ++mid;
            if (*mid < value) {
                first = mid;
                ++first;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return first;
    }

    template <class T, class ForwardIterator>
    ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
        const T& value, random_access_iterator_tag) {
        size_t len = last - first;     //区间长度
        ForwardIterator mid;
        while (len > 0) {
            size_t half = len >> 1;
            mid = first + half;
            if (*mid < value) {
                first = mid + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return first;
    }

    //指向第一个大于value的元素
    template <class T, class ForwardIterator>
    ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
        const T& value) {
        return __upper_bound(first, last, value, iterator_category(first));
    }

    template <class T, class ForwardIterator>
    ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
        const T& value, forward_iterator_tag) {
        size_t len = 0;
        ForwardIterator t = first;
        while (t != last) {
            ++t;
            ++len;
        }
        ForwardIterator mid;
        while (len > 0) {
            size_t half = len >> 1;
            mid = first;
            for (size_t i = 0; i < half; ++i)
                ++mid;
            if (*mid > value)
                len = half;
            else {
                first = mid;
                ++first;
                len = len - half - 1;
            }
        }
        return first;
    }

    template <class T, class ForwardIterator>
    ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
        const T& value, random_access_iterator_tag) {
        size_t len = last - first;
        ForwardIterator mid;
        while (len > 0) {
            size_t half = len >> 1;
            mid = first + half;
            if (*mid > value)
                len = half;
            else {
                first = mid + 1;
                len = len - half - 1;
            }
        }
        return first;
    }

    template <class T, class ForwardIterator>
    pair<ForwardIterator, ForwardIterator>
    equal_range(ForwardIterator first, ForwardIterator last, const T& value) {
        return __equal_range(first, last, value, iterator_category(first));
    }

    template <class T, class ForwardIterator>
    pair<ForwardIterator, ForwardIterator>
    __equal_range(ForwardIterator first, ForwardIterator last,
        const T& value, forward_iterator_tag) {
        size_t len;
        ForwardIterator t = first;
        while (t != last) {
            ++t;
            ++len;
        }
        ForwardIterator mid;
        while (len > 0) {
            size_t half = len >> 1;
            mid = first;
            for (size_t i = 0; i < half; ++i)
                ++mid;
            if (*mid < value) {
                first = mid;
                ++first;
                len = len - half - 1;
            }
            else if (*mid > value)
                len = half;
            else {
                ForwardIterator left = lower_bound(first, mid, value);
                for (size_t i = 0; i < len; ++i)
                    ++first;
                ForwardIterator right = upper_bound(++mid, first, value);
                return pair<ForwardIterator, ForwardIterator>(left, right);
            }
        }
        return pair<ForwardIterator, ForwardIterator>(first, first);
    }

    template <class T, class ForwardIterator>
    pair<ForwardIterator, ForwardIterator>
        __equal_range(ForwardIterator first, ForwardIterator last,
            const T& value, random_access_iterator_tag) {
        size_t len = last - first;
        ForwardIterator mid;
        while (len > 0) {
            size_t half = len >> 1;
            mid = first + half;
            if (*mid < value) {
                first = mid;
                ++first;
                len = len - half - 1;
            }
            else if (*mid > value)
                len = half;
            else {
                ForwardIterator left = lower_bound(first, mid, value);
                ForwardIterator right = upper_bound(++mid, first + len, value);
                return pair<ForwardIterator, ForwardIterator>(left, right);
            }
        }
        return pair<ForwardIterator, ForwardIterator>(first, first);
    }
} 


#endif