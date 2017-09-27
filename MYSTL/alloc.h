#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>

namespace mySTL {
    class alloc {
    private:
        enum { ALIGN = 8 };         //最小区块
        enum { MAX_BYTES = 128 };   //最大区块
        enum { NFREELISTS = MAX_BYTES / ALIGN };    //free_lists个数
        enum { NJOBS = 20 };        //每次增加的节点数

    private:
        //将字节上调至8的倍数
        static size_t ROUND_UP(size_t bytes) {
            return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
        }
        
    private:
        //free_lists 的节点构造
        union obj {
            obj *next;
            char client[1];
        };
        
        static obj *free_list[NFREELISTS];

    private:
        //根据区块的大小决定使用第n个free_list
        static size_t FREELIST_INDEX(size_t bytes) {
            return ((bytes + ALIGN - 1) / ALIGN - 1);
        }
        //返回一个大小为n的对象，并可能加入大小为n的其他对象到free list
        static void *refill(size_t n);
        //配置一大块空间，容纳nobjs * size 大小空间
        //如果不能分配nobjs个空间，nobjs可能降低
        static char *chunk_alloc(size_t size, int &nobjs);
        //out of memory 处理函数   并未设置内存不足处理函数set_new_handle

    private:
        static char *start_free;    //内存池起始位置
        static char *end_free;      //内存池结束位置
        static size_t heap_size;

    public:
        static void *allocate(size_t n);    //空间配置
        static void deallocate(void *p, size_t n);  //空间释放
        static void *reallocate(void *p, size_t old_size, size_t new_size);
    };

}

#endif // !ALLOC_H
