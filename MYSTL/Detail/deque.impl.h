#ifndef DEQUE_IMPL_H_
#define DEQUE_IMPL_H_

namespace mySTL {
    template <class T, class Alloc>
    void deque<T, Alloc>::fill_initialize(size_type n, 
        const value_type& val){
        //分配空间设置节点
        create_map_and_nodes(n);
        //uninitialized_fill(bug)
        for (map_pointer cur = start.node; cur < finish.node; ++cur)
            uninitialized_fill(*cur, *cur + deque_buffer_size(sizeof(T)), val);
        uninitialized_fill(finish.first, finish.cur, val);
    }

    template <class T, class Alloc>
    void deque<T, Alloc>::create_map_and_nodes(size_type n) {
        size_type bufsize = deque_buffer_size(sizeof(T));
        size_type num_nodes = n / bufsize + 1;
        map_size = MIN_NODES > (num_nodes + 2) ? MIN_NODES : (num_nodes + 2);
        map = map_allocator::allocate(map_size);
        //使nstart, nfinish指向nodes中间区段，使头尾都有足够空间
        map_pointer nstart = map + ((map_size - num_nodes) >> 1);
        map_pointer nfinish = nstart + num_nodes - 1;

        for (map_pointer cur = nstart; cur <= nfinish; ++cur)
            *cur = data_allocator::allocate(bufsize);
        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur = finish.first + n % bufsize;
    }

}

#endif