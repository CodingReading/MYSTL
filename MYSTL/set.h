#ifndef SET_H_
#define SET_H_

#include "rb_tree.h"

namespace mySTL {
    template <class Key, class Compare = less<Key>, class Alloc = alloc>
    class set {
    public:
        typedef Key             key_type;
        typedef Key             value_type;
        typedef Compare         key_compare;
        typedef Compare         value_compare;
    private:
        typedef rb_tree<key_type, value_type, identity<value_type>,
            key_compare, Alloc> Container;
        Container   t;      //红黑树实现set

    public:
        typedef typename Container::reference       reference;
        typedef typename Container::iterator        iterator;
        typedef typename Container::size_type       size_type;
        typedef typename Container::difference_type difference_type;
        //构造函数
        set():t(Compare()){}
        explicit set(const Compare& comp):t(comp){}

        template <class InputIterator>
        set(InputIterator first, InputIterator last) : t(Compare()) {
            t.insert_unique(first, last);
        }

        template <class InputIterator>
        set(InputIterator first, InputIterator last, const Compare& comp):t(comp) {
            t.insert_unique(first, last);
        }

        set(const set<Key, Compare, Alloc>& x):t(x.t){}
        set<Key, Compare, Alloc>& operator= (const set<Key, Compare, Alloc>& x) {
            t = x.t;
            return *this;
        }

    public:         //容量相关
        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return t.key_comp(); }
        iterator begin() const { return t.begin(); }
        iterator end() const { return t.end(); }
        bool empty() const { return t.empty(); }
        size_type size() { return t.size(); }

    public:     //操作相关
        pair<iterator, bool> insert(const value_type& x) {
            return t.insert_unique(x);
        }

        template <class InputIterator>
        void insert(InputIterator first, InputIterator last) {
            t.insert_unique(first, last);
        }

        void erase(iterator pos) {
            t.erase(pos);
        }
        
        void earse(const key_type& x) {
            t.erase(x);
        }

        void erase(iterator first, iterator last) {
            t.erase(first, last);
        }

        void clear() {
            t.clear();
        }

        iterator find(const key_type& x) {
            return t.find(x);
        }
        
        size_type count(const key_type& x) {
            return t.count(x);
        }

      /*  iterator lower_bound(const key_type& x) {
            return t.lower_bound(x);
        }

        iterator upper_bound(const key_type& x) {
            return t.upper_bound(x);
        }

        pair<iterator, iterator> equal_range(const key_type& x) {
            return t.equal_range(x);
        }*/
    };
}

#endif // !SET_H_
