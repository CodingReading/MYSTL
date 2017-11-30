#ifndef RB_TREE_IMPL_H_
#define RB_TREE_IMPL_H_

namespace mySTL {
    //---------------------------------构造--------------------------------
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& 
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator= (const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) {
        if (this != &x) {
            clear();
            node_count = 0;
            if (x.root() == nullptr) {
                header->parent = nullptr;
                leftmost() = header;
                rightmost() = header;
            }
            else {
                root() = __copy(x.root(), header);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
                node_count = x.node_count;
            }
        }
        return *this;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
        if (node_count != 0) {
            __erase(root());
            root() = nullptr;
            node_count = 0;
            leftmost() = nullptr;
            rightmost() = nullptr;
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(link_type x, link_type p) {
        if (x == nullptr)
            return nullptr;
        link_type root = clone_node(x);
        root->parent = p;
        root->right = __copy(x->right, x);
        root->left = __copy(x->left, x);
        return root;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x) {
        if (x != nullptr) {
            destroy_node(x);
            if (x->right != nullptr)
                __erase(x->right);
            if (x->left != nullptr)
                __erase(x->left);
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v) {
        link_type p = header;
        link_type x = root();
        
        while (x != nullptr) {
            p = x;
            x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
        }
        return __insert(x, p, v);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
        link_type p = header;
        link_type x = root();
        bool comp = true;
        while (x != nullptr) {
            p = x;
            comp = key_compare(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }

        iterator j(p);
        if (comp)
            if (p == begin()) { //如果插入点父节点为最左子节点
                return pair<iterator, bool>(__insert(x, p, v), true);
            }
            else
                --j;    //减小j，看是否有重复

        //新节点Key不重复
        if (key_compare(key(j.node), KeyOfValue()(v)))
            return pair<iterator, bool>(__insert(x, p, v), true);
        //Key重复
        return pair<iterator, bool>(j, false);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_left(link_type x, link_type& root) {
        //x为旋转支点，将x右节点左旋，y顶替x
        link_type y = x->right;
        x->right = y->left;

        //设父节点
        if (y->left != nullptr)
            y->left->parent = x;

        if (x == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        //父子颠倒
        y->left = x;
        x->parent = y;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_right(link_type x, link_type& root) {
        //x为旋转支点，将x左节点右旋，y顶替x
        link_type y = x->left;
        x->left = y->right;

        if (y->right != nullptr)
            y->right->parent = x;

        if (x == root)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        //父子颠倒
        y->right = x;
        x->parent = y;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(link_type x, link_type y, const value_type& x {
        link_type z = creat_node(x);
        
        if (y == header || x != nullptr || key_compare(KeyOfValue()(v), key(y))) {
            //左插
            left(y) = z;
            if (y == header) {
                root() = z;
                rightmost() = z;
            }
            else if (y == leftmost())
                leftmost() = z;
        }
        else {  //右插
            right(y) = z;
            if (y == rightmost())
                rightmost() = z;
        }

        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;

        rb_tree_rebalance_for_insert(z, header->parent);
        ++node_count;
        return iterator(z);
    } 

}




#endif