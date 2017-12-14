#ifndef RB_TREE_H_
#define RB_TREE_H_

#include "pair.h"
#include "allocator.h"
#include "construct.h"
#include "algorithm.h"
#include "iterator.h"
#include "functional.h"

namespace mySTL {
    typedef bool rb_tree_color_type;
    const rb_tree_color_type rb_tree_red = false;
    const rb_tree_color_type rb_tree_black = true;

    //不采用继承
    template <class T>
    struct rb_tree_node {
        typedef rb_tree_color_type  color_type;
        typedef rb_tree_node*       link_type;
        
        color_type      color;
        link_type       parent;
        link_type       left;
        link_type       right;
        T               value;

        static link_type minimum(link_type p) {
            while (p->left != nullptr)
                p = p->left;
            return p;
        }

        static link_type maximum(link_type p) {
            while (p->right != nullptr)
                p = p->right;
            return p;
        }
    };

    template <class T>
    class rb_tree_iterator {
    public:
        typedef T                           value_type;
        typedef T&                          reference;
        typedef T*                          pointer;
        typedef ptrdiff_t                   difference_type;
        typedef bidirectional_iterator_tag  iterator_category;
        typedef rb_tree_iterator<T>         self;
        typedef rb_tree_iterator<T>         iterator;
        typedef rb_tree_node<T>*            link_type;
    public:     //构造
        rb_tree_iterator() {}
        rb_tree_iterator(link_type x) { node = x; }
        rb_tree_iterator(const iterator& it) { node = it.node; }

    public:     //访问
        reference operator* () const { return node->value; }
        pointer operator-> () const { return &(operator*()); }

        //前置++
        self& operator++() {
            if (node->right != nullptr) {
                //右子树的最左子节点
                node = node->right;
                while (node->left != nullptr)
                    node = node->left;
            }
            else {
                //没有右子节点，则一直上溯，直到当前节点是左子节点
                link_type p = node->parent;
                while (p->right == node) {
                    node = p;
                    p = p->parent;
                }
                //若寻找根节点下一个下一个节点且根节点无右子则为end
                if (node->right != p)
                    node = p;       
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
            //头结点
            if (node->color == rb_tree_red && node == node->parent->parent)
                node = node->right;
            else if (node->left != nullptr) {
                //左子树最右
                link_type t = node->left;
                while (t->right != nullptr)
                    t = t->right;
                node = t;
            }
            else {
                //上溯，直到当前为右子节点
                link_type p = node->parent;
                while (p->left == node) {
                    node = p;
                    p = p->parent;
                }
                node = p;
            }
            return *this;
        }

        //后置--
        self operator-- (int) {
            self t = *this;
            --*this;
            return t;
        }

    public:
        link_type        node;
    };

    template <class T>
    inline bool operator== (const rb_tree_iterator<T>& x, const rb_tree_iterator<T>& y) {
        return x.node == y.node;
    }

    template <class T>
    inline bool operator!= (const rb_tree_iterator<T>& x, const rb_tree_iterator<T>& y) {
        return x.node != y.node;
    }

    //红黑树
    template <class Key, class Value, class KeyOfValue,
                class Compare, class Alloc = alloc>
    class rb_tree {
    protected:
        typedef rb_tree_node<Value>             tree_node;
        typedef allocator<tree_node, Alloc>     rb_tree_node_allocator;
        typedef rb_tree_color_type              color_type;

    public:
        typedef Key                         key_type;
        typedef Value                       value_type;
        typedef value_type&                 reference;
        typedef tree_node*                  link_type;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;
        typedef rb_tree_iterator<Value>     iterator;

    protected:
        link_type get_node() {
            return rb_tree_node_allocator::allocate();
        }

        void delete_node(link_type p) {
            rb_tree_node_allocator::deallocate(p);
        }

        link_type creat_node(const value_type& x) {
            link_type temp = get_node();
            construct(&temp->value, x);
            return temp;
        }

        //复制节点值和颜色
        link_type clone_node(link_type x) {
            link_type temp = creat_node(x->value);
            temp->color = x->color;
            temp->left = nullptr;
            temp->right = nullptr;
            temp->parent = nullptr;
            return temp;
        }

        void destroy_node(link_type x) {
            destroy(&x->value);
            delete_node(x);
        }

    protected:
        size_type   node_count;
        link_type   header;         //头指针
        Compare     key_compare;

        link_type& root() const { return header->parent; }
        link_type& leftmost() const { return header->left; }
        link_type& rightmost() const { return header->right; }

        //取节点成员
        static link_type& left(link_type x) { return x->left; }
        static link_type& right(link_type x) { return x->right; }
        static link_type& parent(link_type x) { return x->parent; }
        static reference value(link_type x) { return x->value; }
        static color_type& color(link_type x) { return x->color; }
        static const Key& key(link_type x) { return KeyOfValue()(value(x)); }

        //极值
        static link_type minimum(link_type x) {
            return tree_node::minimum(x);
        }

        static link_type maximum(link_type x) {
            return tree_node::maximum(x);
        }

    public:     //构造析构
        rb_tree(const Compare& comp = Compare())
            : node_count(0), key_compare(comp) {
            init();
        }
        //拷贝构造
        rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
            : key_compare(x.key_compare), node_count(0) {
            if (x.root() == nullptr)
                init();
            else {
                header = get_node();
                color(header) = rb_tree_red;
                root() = __copy(x.root(), header);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
            }
            node_count = x.node_count;
        }
        //拷贝赋值
        rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& operator= (
            const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);

        ~rb_tree() {
            clear();
            delete_node(header);
        }

    public:             //操作相关
        //键值不可重复
        pair<iterator, bool> insert_unique(const value_type& x);
        template <class InputIterator>
        void insert_unique(InputIterator first, InputIterator last);
        //键值可重复
        iterator insert_equal(const value_type& x);
        void erase(iterator pos);
        void erase(iterator first, iterator last);
        void erase(const key_type& x);
        iterator find(const key_type& x);
        size_type count(const key_type& x);
        iterator lower_bound(const key_type& x);
        iterator upper_bound(const key_type& x);
        pair<iterator, iterator> equal_range(const key_type& x);

    public:                 //容量相关
        Compare key_comp() const { return key_comp; }
        iterator begin() const { return leftmost(); }
        iterator end() const { return header; }
        bool empty() const { return node_count == 0; }
        size_type size() const { return node_count; }
        void clear();

    private:
        iterator __insert(link_type x, link_type y, const value_type& v);
        link_type __copy(link_type x, link_type p);
        void __erase(link_type x);          //删除以x为根的树
        void init() {
            header = get_node();
            color(header) = rb_tree_red;    //header为红
            root() = nullptr;
            leftmost() = header;            //左右节点指向自己
            rightmost() = header;
        }
        //左旋
        inline void rb_tree_rotate_left(link_type x);
        //右旋
        inline void rb_tree_rotate_right(link_type x);
        //插入，再平衡
        void rb_tree_rebalance_for_insert(link_type x);
        //删除，再平衡
        void rb_tree_rebalance_for_erase(link_type x, link_type parent_of_x);
    };
}

#include "Detail\rb_tree.impl.h"

#endif