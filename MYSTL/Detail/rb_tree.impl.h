#ifndef RB_TREE_IMPL_H_
#define RB_TREE_IMPL_H_

namespace mySTL {
    //---------------------------------拷贝构造--------------------------------
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

    //------------------------------操作函数-------------------------------
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
        if (node_count != 0) {
            __erase(root());
            root() = nullptr;
            node_count = 0;
            leftmost() = header;
            rightmost() = header;
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
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator pos) {
        link_type x = nullptr;
        link_type y = pos.node;     
        link_type z = y;            //要删除的节点
        link_type px = nullptr;     //parent of x

        //若z左右子树都有，把z替换成左子树的最右节点(右子树最左节点算法类似)
        if (y->left == nullptr)
            x = y->right;
        else if (y->right == nullptr)
            x = y->left;
        else {      //左右子树都有
            y = y->left;
            while (y->right != nullptr)
                y = y->right;
            x = y->left;
        }
        
        if (y == z) {   //当待删除点pos最多只有一个子树时
            px = y->parent;
            if (x != nullptr)   //具有一颗子树
                x->parent = z->parent;
            if (root() == z)
                root() = x;
            else if (z == z->parent->left)
                z->parent->left = x;
            else
                z->parent->right = x;

            if (z == leftmost())
                leftmost() = (x == nullptr ? z->parent : minimum(x));
            if (z == rightmost())
                rightmost() = (x == nullptr ? z->parent : maximum(x));
        }
        else {  //z有两个子树
            //z的右子树接到y的右子树
            z->right->parent = y;
            y->right = z->right;
            if (y != z->left) {
                //y的左子树接到z左子树的右子树
                px = y->parent;
                z->left->right = y->left;
                if (x != nullptr)
                    x->parent = z->left;
                //z左子树接到y的左子树
                y->left = z->left;
                y = z->left->parent;
            }
            else
                px = y;
            
            if (z == root())
                root() = y;
            else if (z == z->parent->left)
                y = z->parent->left;
            else
                y = z->parent->right;

            y->parent = z->parent;
            mySTL::swap(y->color, z->color);
            y = z;
        }

        /* 若顶替pos的节点是黑节点, 因为将它删除了，
        所以包含x的路径少了一个黑节点，引发不平衡 */
        if (y->color == rb_tree_black)
            rb_tree_rebalance_for_erase(x, px);
        destroy_node(y);
        --node_count;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
        if (first == begin() && last == end())
            clear();
        else {
            while (first != last)
                erase(first++);
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& x) {
        pair<iterator, iterator> p = equal_range(begin(), end(), x);
        erase(p.first, p.second);
    }
    //------------------------------底层实现函数--------------------------
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
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_left(link_type x) {
        //x为旋转支点，将x右节点左旋，y顶替x
        link_type y = x->right;
        x->right = y->left;

        //设父节点
        if (y->left != nullptr)
            y->left->parent = x;
        y->parent = x->parent;

        if (x == root())
            root() = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        //父子颠倒
        y->left = x;
        x->parent = y;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_right(link_type x) {
        //x为旋转支点，将x左节点右旋，y顶替x
        link_type y = x->left;
        x->left = y->right;

        if (y->right != nullptr)
            y->right->parent = x;
        y->parent = x->parent;

        if (x == root())
            root() = y;
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
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(link_type x, link_type y, const value_type& v) {
        link_type z = creat_node(v);
        
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

        rb_tree_rebalance_for_insert(z);
        ++node_count;
        return iterator(z);
    } 

    /*
        三种不平衡：  
            1. 当前节点父节点为红，叔叔节点为红
            2. 当前节点父节点为红，叔叔节点为黑，当前节点为父节点右子
            3. 当前节点父节点为红，叔叔节点为黑，当前节点为父节点左子
            
        对应解法：     (父节点为祖父节点左子树情况下， 右子树类似)
            1. 父节点和叔叔节点涂黑，祖父节点涂红，祖父节点为新的当前节点
            2. 父节点为新的当前节点，以新的当前节点为支点左旋，此后必会
               导致出现情况3
            3. 父节点涂黑，祖父节点涂红，以祖父节点为支点右旋
            
    */
    
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance_for_insert(link_type x) {
        x->color = rb_tree_red;
        while (x != root() && x->parent->color == rb_tree_red) {
            //父节点为左子节点
            if (x->parent == x->parent->parent->left) {
                link_type uncle = x->parent->parent->right;
                if (uncle != nullptr && uncle->color == rb_tree_red) {
                    //case1
                    x->parent->color = rb_tree_black;
                    uncle->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    x = x->parent->parent;
                }
                else {
                    //case2
                    if (x == x->parent->right) {
                        x = x->parent;
                        rb_tree_rotate_left(x);
                    }
                    //case3
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    rb_tree_rotate_right(x->parent->parent);
                }
            }
            else {  //父节点为右子
                //case1
                link_type uncle = x->parent->parent->left;
                if (uncle != nullptr && uncle->color == rb_tree_red) {
                    uncle->color = rb_tree_black;
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    x = x->parent->parent;
                }
                else {
                    //case2
                    if (x == x->parent->left) {
                        x = x->parent;
                        rb_tree_rotate_right(x);
                    }
                    //case3
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    rb_tree_rotate(x->parent->parent);
                }
            }
        }
        root()->color = rb_tree_black;
    }

    //包含x的路径上少了一个黑节点
    /*
        四种不平衡情况: 
            1. 当前节点为黑，兄弟为红
            2. 当前节点为黑，兄弟为黑，兄弟节点的两个子节点都为黑
            3. 当前节点为黑，兄弟为黑，兄弟右子为红，兄弟左子为黑
            4. 当前节点为黑，兄弟为黑，兄弟左子为红，右子为任意色
        对应解法: (当前节点为右子树时，为左子树算法类似)
            1. 兄弟涂黑，父节点涂红，以当前节点父节点为支点右旋(转化为
               兄弟节点为黑的情况)
            2. 兄弟涂红，当前节点移到当前节点父节点
            3. 兄弟涂红，兄弟右子涂黑，以兄弟为支点左旋，转到情况四
            4. 当前节点兄弟节点涂成父节点颜色，父节点涂黑，兄弟左子涂黑
               以当前节点父节点为支点右旋，
        解法的所有目的都是为了把当前节点父节点之下的路径的黑节点数调到平衡(举个例
        子，若当前节点为右子树，因为之前删除的当前节点的父节点是黑色的，则现在当
        前节点的父节点往右所有的路径上黑色节点的个数减少了1，我们通过变换把当前
        节点父节点左子树上所有的黑色节点也减少1，则包含当前节点父节点的所有路径
        黑节点数少1， 把当前节点移到当前节点父节点，如果为红或者为根，则把它涂黑
        结束，如果为黑则继续这些步骤)
    */
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance_for_erase(link_type x, link_type parent_of_x) {
        while (x != root() && (x == nullptr || x->color == rb_tree_black)) {
            //当前节点为右子树
            if (x == parent_of_x->right) {
                link_type silbing = parent_of_x->left;
                // case1
                if (silbing->color == rb_tree_red) {
                    silbing->color = rb_tree_black;
                    parent_of_x->color = rb_tree_red;
                    rb_tree_rotate_right(parent_of_x);
                    silbing = parent_of_x->left;
                }

                // case2
                if ((silbing->left == nullptr || silbing->left->color == rb_tree_black) && 
                    (silbing->right == nullptr || silbing->right->color == rb_tree_black)) {
                    silbing->color = rb_tree_red;
                    x = parent_of_x;
                    parent_of_x = parent_of_x->parent;
                }
                else {
                    //case3
                    if (silbing->left == nullptr ||
                        silbing->left->color == rb_tree_black) {
                        //if (silbing->right != nullptr)
                        silbing->right->color = rb_tree_black;
                        silbing->color = rb_tree_red;
                        rb_tree_rotate_left(silbing);
                        silbing = parent_of_x->left;
                    }
                    //case4
                    silbing->color = parent_of_x->color;
                    parent_of_x->color = rb_tree_black;
                    silbing->left->color = rb_tree_black;
                    rb_tree_rotate_right(parent_of_x);
                    break;
                }
            }
            else {      //当前节点为左子树
                link_type silbing = parent_of_x->right;
                // case1
                if (silbing->color == rb_tree_red) {
                    silbing->color = rb_tree_black;
                    parent_of_x->color = rb_tree_red;
                    rb_tree_rotate_left(parent_of_x);
                    silbing = parent_of_x->right;
                }

                // case2
                if ((silbing->left == nullptr || silbing->left->color == rb_tree_black) &&
                    (silbing->right == nullptr || silbing->right->color == rb_tree_black)) {
                    silbing->color = rb_tree_red;
                    x = parent_of_x;
                    parent_of_x = parent_of_x->parent;
                }
                else {
                    // case3
                    if (silbing->right == nullptr ||
                        silbing->right->color == rb_tree_black) {
                        silbing->color = rb_tree_red;
                        silbing->left->color = rb_tree_red;
                        rb_tree_rotate_right(silbing);
                        silbing = parent_of_x->right;
                    }
                    // case4
                    silbing->color = parent_of_x->color;
                    parent_of_x->color = rb_tree_black;
                    silbing->right->color = rb_tree_black;
                    rb_tree_rotate_left(parent_of_x);
                    break;
                }
            }
        }

        if (x != nullptr)
            x->color = rb_tree_black;
    }
}

#endif