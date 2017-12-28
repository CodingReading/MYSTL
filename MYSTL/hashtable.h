/*
    hashtable:  vector为桶，散列值相同的元素以链表形式连接在每个桶上
*/
#include "vector.h"

namespace mySTL {
    //节点
    template <class Value>
    struct hashtable_node {
        Value           value;
        hashtable_node* next;
    };

    const int mySTL_num_primers = 28;
    const unsigned int mySTL_primers_list[mySTL_num_primers] = {
        53,         97,         193,       389,       769,
        1543,       3079,       6151,      12289,     24593,
        49157,      98317,      196613,    393241,    786433,
        1572869,    3145739,    6291469,   12582917,  25165843,
        50331653,   100663319,  201326611, 402653189, 805306457,
        1610612741, 3221225473, 4294967291
    };

    inline unsigned int mySTL_next_prime(unsigned int n) {
        const unsigned int* pos = lower_bound(mySTL_primers_list, mySTL_primers_list
            + mySTL_num_primers, n);
        return pos == (mySTL_primers_list + mySTL_num_primers) ?
            *(mySTL_primers_list + mySTL_num_primers - 1) : *pos;
    }

    template <class Value, class Key, class HashFcn, class ExtractKey,
        class EqualKey, class Alloc = alloc>
    class hashtable {
    public:
        typedef HashFcn                 hasher;
        typedef EqualKey                key_equal;
        typedef size_t                  size_type;
        typedef Value                   value_type;
        typedef hashtable_node<Value>   node;
        typedef allocator<node, Alloc>  node_allocator;
        typedef hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
    private:
        hasher      hash;
        key_equal   equals;
        ExtractKey  get_key;
        vector<node*, Alloc> buckets;
        size_type            num_elements;

    public:         //构造析构
        hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) :
            hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
            initialize_buckets(n);
        }

        hashtable(const hashtable& other) :
            hash(other.hash), equals(other.equals), get_key(other.get_key), num_elements(0) {
            copy_from(other);
        }

        hashtable& operator= (const hashtable& other) {
            if (this != &other) {
                hash = other.hash;
                equals = other.equals;
                get_key = other.get_key;
                copy_from(other);
            }
            return *this;
        }

        ~hashtable() {
            clear();
        }

    public:     //容量相关
        size_type size() const {
            return num_elements;
        }
        iterator begin() {
            for (size_type n = 0; n < buckets.size(); ++n)
                if (buckets[n] != nullptr)
                    return iterator(buckets[n], this);
            return end();
        }
        iterator end() { return iterator(nullptr, this); }

        size_type num_buckets() const { return buckets.size(); }

    public:     //操作相关
        pair<iterator, bool> insert_unique(const value_type& obj);
        iterator insert_equal(const value_type& obj);
        pair<iterator, bool> insert_unique_noresize(const value_type& obj);
        iterator insert_equal_noresize(const value_type& obj);

    private:
        node* get_node() {
            return (node*)node_allocator::allocate();
        }

        void delete_node(node* p) {
            node_allocator::deallocate(p);
        }

        node* create_node(const value_type& v) {
            node* t = get_node();
            construct(t, &v);
            return t;
        }

        void destroy_node(node* p) {
            destroy(&p->value);
            delete_node(p);
        }

        void resize(size_type num_elements);
    };

    template <class Value, class Key, class HashFcn, class ExtractKey, 
        class EqualKey, class Alloc = alloc>
    struct hashtable_iterator {
        typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
        typedef hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
        typedef hashtable_node<Value>   node;
        typedef forward_iterator_tag    iterator_category;
        typedef Value                   value_type;
        typedef ptrdiff_t               difference_type;
        typedef size_t                  size_type;
        typedef Value&                  reference;

        node*       cur;
        hashtable*  ht;

        hashtable_iterator(){}
        hashtable_iterator(node* n, hashtable* tab):cur(n), ht(tab){}
        reference operator* () const {
            return cur->value;
        }

        //前置++
        iterator& operator++() {
            const node* old = cur;
            cur = cur->next;
            if (cur == nullptr) {
                size_type bucket = ht->bkt_num(old->value);
                while (cur == nullptr && ++bucket < ht->buctet.size())
                    cur = ht->buckets[bucket];
            }
            return *this;
        }
        //后置++
        iterator operator++(int) {
            iterator t = *this;
            ++*this;
            return t;
        }

        bool operator== (const iterator& other) const {
            return cur == other.cur;
        }

        bool operator!= (const iterator& other) const {
            return cur != other.cur;
        }
    };
}


#include "Detail/hashtable.impl.h"