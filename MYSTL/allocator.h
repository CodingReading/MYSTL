/*
    allocator : 为所有容器提供接口
                对空间配置器alloc的封装
*/

#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "alloc.h"

namespace mySTL {
    template < class T, class Alloc = alloc >
    class allocator {
    public :
        static T *allocate(size_t n) {
            return n == 0 ? 0 : (T*)Alloc::allocate(n * sizeof(T));
        }

        static T* allocate(void) {
            return (T*)Alloc::allocate(sizeof(T));
        }

        static void deallocate(T *p, size_t n) {
            if (n != 0)
                Alloc::deallocate(p, n * sizeof(T));
        }

        static void deallocate(T *p) {
            Alloc::deallocate(p, sizeof(T));
        }
    };
}


#endif	

