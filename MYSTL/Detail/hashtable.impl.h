#ifndef HASHTABLE_IMPL_H_
#define HASHTABLE_IMPL_H_

namespace mySTL {
    // 不允许键值重复
    template <class Value, class Key, class HashFcn, class ExtractKey,
    class EqualKey, class Alloc = alloc>
    pair<hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, 
        Alloc>::insert_unique(const value_type& obj) {
        resize(num_elements + 1);
        return insert_unique_noresize(obj);
    }

    // 允许键值重复
    template <class Value, class Key, class HashFcn, class ExtractKey,
    class EqualKey, class Alloc = alloc>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey,
        Alloc>::insert_equal(const value_type& obj) {
        resize(num_elements + 1);
        return insert_equal_noresize(obj);
    }

    // 超过当前容量，再分配
    template <class Value, class Key, class HashFcn, class ExtractKey,
    class EqualKey, class Alloc = alloc>
    void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey,
        Alloc>::resize(size_type num_elements) {
        size_type oldsize = buckets.size();
        if (num_elements > oldsize) {
            size_type n = mySTL_next_prime(num_elements);
            if (n > oldsize) {
                vector<node*> temp(n, nullptr);
                // 移动节点到新桶
                for (size_type count = 0; count < oldsize; ++count) {
                    node* cur = buckets[count];
                    while (cur != nullptr) {
                        size_type new_bucket = bkt_num(cur->value, n);
                        buckets[count] = cur->next;
                        cur->next = temp[new_bucket];
                        temp[new_bucket] = cur;
                        cur = buckets[count];
                    }
                }
                buckets.swap(temp);
            }
        }
    }

    // 在不用重分配的前提下插入，键值不重复
    template <class Value, class Key, class HashFcn, class ExtractKey,
    class EqualKey, class Alloc = alloc>
    pair<hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, 
        Alloc>::insert_unique_noresize(const value_type& obj) {
        size_type n = bkt_num(obj);
        for (node *cur = buckets[n]; cur != nullptr; cur = cur->next)
            if (equals(get_key(obj), get_key(cur->value))
                return pair<iterator, bool>(iterator(cur, this), false);
        node* temp = create_node(obj);
        temp->next = buckets[n];
        buckets[n] = temp;
        ++num_elements;
        return pair<iterator, bool>(iterator(temp, this), true);
    }

    // 在不用重分配的前提下插入，键值可重复
    template <class Value, class Key, class HashFcn, class ExtractKey,
    class EqualKey, class Alloc = alloc>
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator 
    hashtable<Value, Key, HashFcn, ExtractKey, EqualKey,
        Alloc>::insert_equal_noresize(const value_type& obj) {
        size_type n = bkt_num(obj);
        for (node* cur = buckets[n]; cur != nullptr; cur = cur->next)
            if (equals(get_key(obj), get_key(cur->value))) {
                node *temp = create_node(obj);
                temp->next = cur->next;
                cur->next = temp;
                ++num_elements;
                return iterator(temp, this);
            }
        node *temp = create_node(obj);
        temp->next = buckets[n];
        buckets[n] = temp;
        ++num_elements;
        return iterator(temp, this);
    }
}


#endif