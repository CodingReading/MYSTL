#ifndef VECTOR_H_
#define VECTOR_H_

#include "construct.h"
#include "uninitialized.h"
#include "algorithm.h"
#include "allocator.h"
#include "iterator.h"



namespace mySTL {
    template <class T, class Alloc = allocator<T>>
    class vector {
    public:
        typedef T				value_type;
        typedef T*				iterator;
        typedef T&				reference;
        typedef iterator		pointer;
        typedef size_t			size_type;
        typedef ptrdiff_t		difference_type;

    private:
        iterator	start;
        iterator	finish;
        iterator	end_of_storage;
        typedef		Alloc		data_allocator;
    public:			//构造析构相关
        vector() :start(0), finish(0), end_of_storage(0) {}
        explicit vector(const size_type n);
        vector(const size_type n, const T& value);

        vector(const vector<T, Alloc>& v);
        //注释后能编译通过
        //此构造函数必须对InputIterator 判断是否是整数，否则会出错
        //真正的执行函数为vector_aux
        template <class InputIterator>
        vector(InputIterator first, InputIterator last);

        ~vector();
        vector<T>& operator= (const vector<T>& v);

    public:		//成员访问相关
        reference operator[] (size_type index) { return *(start + index); }
        reference front() { return *start; }
        reference back() { return *(finish - 1); }

    public:		//获取迭代器
        iterator begin() { return start; }
        iterator end() { return finish; }

    public:		//容量相关
        bool empty() { return start == finish; }
        size_type size() const { return finish - start; }
        size_type capacity() const { return end_of_storage - start; }
        void reserve(size_type new_cap);
        void shrink_to_fit();	//回收多余空间

    public:		//容器操作相关
        void clear();
        iterator insert(iterator pos, size_type n, const T& val);
        iterator insert(iterator pos, const T& val);
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        void push_back(const T& val);
        void pop_back();
        void resize(size_type n);
        void swap(vector<T>& other);

    public:		//字典序比较相关
        bool operator== (const vector<T, Alloc>& v);
        bool operator< (const vector<T, Alloc>& v);
        bool operator> (const vector<T, Alloc>& v);
        bool operator!= (const vector<T, Alloc>& v);
        bool operator>= (const vector<T, Alloc>& v);
        bool operator<= (const vector<T, Alloc>& v);
    private:
        void allocate_and_copy(size_type n, iterator begin, iterator end);
        void allocate_and_fill(size_type n, const T& value);
        void destroy_and_deallocate();

        template <class InputIterator>
        void vector_aux(InputIterator first, InputIterator last, std::false_type);

        //虽然传入的确实是指针，但是按照我的理解，编译器并不知道是不是，所以
        //对于是否是整数需要有true_type false_type,
        //没有这个函数无法编译通过
        template <class Integer>
        void vector_aux(Integer n, const value_type &val, std::true_type);
    };
}

#include "Detail/vector.impl.h"
#endif
