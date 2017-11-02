/*
    deque 双向开口的容器
    通过中控器map(存储指针的数组)维护多段连续的内存空间，使deque使用起来就像是一段连续的空间
    deque中  start.cur指向存储区首个元素
             finish.cur指向存储区尾元素下一个
*/
//因为会进行迭代器安全检查
//VS下需要添加预处理器定义 _ITERATOR_DEBUG_LEVEL=0

#ifndef DEQUE_H_
#define DEQUE_H_

#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"
#include "algorithm.h"
#include "iterator.h"

#define MIN_NODES   8

namespace mySTL {
    inline size_t deque_buffer_size(size_t size) {
        return (size < 512 ? (512 / size) : size_t(1));
    }

    template <class T>
    class deque_iterator {
    public:
        typedef deque_iterator<T>           iterator;
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef ptrdiff_t                   difference_type;
        typedef T**                         map_pointer;
        typedef deque_iterator              self;

        T*  cur;            //指向所指缓冲区当前元素
        T*  first;          //首元素
        T*  last;           //尾元素后一个
        map_pointer node;   //指向控制中心

        deque_iterator():cur(0), first(0), last(0), node(0){}
        deque_iterator(const deque_iterator& other):
            cur(other.cur), first(other.first), last(other.last), node(other.node){}

        static size_t buffer_size() {
            return deque_buffer_size(sizeof(T));
        }

        //重新设置指向管控中心的指针，修改first, last指向缓冲区
        void set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = *new_node + (difference_type)buffer_size();
        }

        reference operator* () const {
            return *cur;
        }

        pointer operator-> () const {
            return &(operator*());
        }

        difference_type operator- (const self& x) const {
            return difference_type((buffer_size() * (node - x.node - 1))
                + (cur - first) + (x.last - x.cur));
        }
        //前置++
        self& operator++ () {
            ++cur;
            if (cur == last) {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        //后置++
        self operator++ (int) {
            self temp = *this;
            ++*this;
            return temp;
        }
        //前置--
        self& operator-- () {
            if (cur == first) {
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
        //后置--
        self operator-- (int) {
            self temp = *this;
            --*this;
            return temp;
        }

        //实现随机存取
        self& operator+= (difference_type n) {
            difference_type offset = n + cur - first;
            size_t size = buffer_size();
            if (offset >= 0 && offset < size)
                cur += n;
            else {
                //不在同一缓冲区内
                difference_type node_offset = offset > 0 ?
                    offset / size :
                    -difference_type((-offset - 1) / size) - 1;
                //重置node,设置cur指向
                set_node(node + node_offset);
                cur = first + (offset - node_offset * size);//offset正负都适用
            }
            return *this;
        }

        self operator+ (difference_type n) const {
            self temp = *this;
            return temp += n;
        }

        self& operator-= (difference_type n) {
            return *this += -n;
        }

        self operator- (difference_type n) const {
            self temp = *this;
            return temp -= n;
        }

        //重载[]，随机存取
        reference operator[] (difference_type n) const {
            return *(*this + n);
        }

        bool operator== (const self& x) const { return cur == x.cur; }
        bool operator!= (const self& x) const { return cur != x.cur; }
        bool operator< (const self& x) const {
            return (node == x.node) ? (cur < x.cur) : (node < x.node);
        }
    };

    template <class T, class Alloc = allocator<T>>
    class deque {
    public:
        typedef T                   value_type;
        typedef T*                  pointer;
        typedef T&                  reference;
        typedef size_t              size_type;
        typedef ptrdiff_t           difference_type;
        typedef deque_iterator<T>   iterator;

    protected:
        typedef T**                 map_pointer;
        typedef Alloc               data_allocator; //每次分配一个元素大小
        typedef allocator<pointer>  map_allocator;//每次配置一个指针大小

    private:
        iterator start;         //表示第一个节点
        iterator finish;        //最后一个节点
        map_pointer map;        //指向map,map中的元素为指向缓冲区的指针
        size_type map_size;     //map能存的指针个数

    public: //构造析构
        deque() {
            create_map_and_nodes(0);
        }
        deque(size_type n, const value_type& val)
            :map(0), map_size(0) {
            fill_initialize(n, val);
        }
        
        deque(const deque& other);
        template <class InputIterator>
        deque(InputIterator first, InputIterator last); 
        deque<T, Alloc>& operator= (deque<T, Alloc>& other);
        ~deque();

    public: //成员访问相关
        iterator begin() { return start; }
        iterator end() { return finish; }
        reference front() const { return *start; }
        reference back() const { return *(finish - 1); }
        reference operator[] (size_type n) {
            return start[n];
        }

    public:     //容量相关
        bool empty() const { return start == finish; }
        size_type size() const { return finish - start; }
     
    public:     //容器操作相关
        void clear();
        iterator insert(iterator pos, size_type n, const value_type& val);

        template <class InputIterator>
        iterator insert(iterator pos, InputIterator first, InputIterator last);

        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        void push_back(const value_type& val);
        void pop_back();
        void push_front(const value_type& val);
        void pop_front();
        void swap(deque<T, Alloc>& other);

    private:
        void fill_initialize(size_type n, const value_type& val);
        void create_map_and_nodes(size_type n);
        void push_back_aux(const value_type& val);
        void push_front_aux(const value_type& val);
        void pop_back_aux();
        void pop_front_aux();
        void new_elements_at_front(size_type n);
        void new_elements_at_back(size_type n);
        //检查是否是整数类型
        template <class InputIterator>
        void deque_aux(InputIterator first, InputIterator last, std::false_type);

        template <class Integer>
        void deque_aux(Integer n, const value_type& val, std::true_type);
        //检查是否是整数类型
        template <class InputIterator>
        iterator insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type);

        template <class Integer>
        iterator insert_aux(iterator pos, Integer n, const value_type& val, std::true_type);

        template <class InputIterator>
        iterator insert_aux(iterator pos, InputIterator first, InputIterator last, size_type n);

        iterator insert_aux(iterator pos, size_type n, const value_type& val);
        
        iterator reserve_elems_at_front(size_type n);
        iterator reserve_elems_at_back(size_type n);
        //默认增加一个节点
        void reserve_map_at_back(size_type nodes_to_add = 1);
        void reserve_map_at_front(size_type nodes_to_add = 1);
        void reallocate_map(size_type nodes_to_add, bool add_at_front);
        void destroy_and_deallocate();
    };
};


#include "Detail\deque.impl.h"
#endif
