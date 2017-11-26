#ifndef RB_TREE_IMPL_H_
#define RB_TREE_IMPL_H_

namespace mySTL {
    //---------------------------------ππ‘Ï--------------------------------
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


}




#endif