/*
* alloc.c
* 实现内存池，空间分配
*/

#include "alloc.h"

namespace MYSTL {
	char *alloc::start_free = 0;
	char *alloc::end_free = 0;
	size_t alloc::heap_size = 0;
	alloc::obj *alloc::free_list[NFREELISTS] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	void *alloc::allocate(size_t n) {
		//大于128调用malloc
		if (n > MAX_BYTES) {
			return (malloc(n));
		}

		size_t index = FREELIST_INDEX(n);
		obj *list = free_list[index];

		//如果有可用空间
		if (list) {
			free_list[index] = list->next;
			return list;
		}
		else {	//无可用空间
			return refill(ROUND_UP(n));
		}
	}

	void alloc::deallocate(void *p, size_t n) {
		if (n > MAX_BYTES) {
			free(p);
			return;
		}

		//区块回收
		size_t index = FREELIST_INDEX(n);
		obj *q = static_cast<obj *> (p);
		q->next = free_list[index];
		free_list[index] = q;
	}
	
	void *alloc::reallocate(void *p, size_t old_size, size_t new_size) {
		deallocate(p, old_size);
		p = allocate(new_size);
		return p;
	}

	//缺省取得20个新区块，如果内存池空间不足，可能小于20
	void* alloc::refill(size_t n) {
		int nobjs = 20;
		//调用chunk_alloc，尝试获得nobjs个区块作为free_list的新节点
		//nobjs是传递引用
		char *chunk = chunk_alloc(n, nobjs);

		//只获得一个区块则分配给调用者
		if (nobjs == 1) {
			return chunk;
		}

		void *result = chunk;	//此块空间返回给调用者
		obj *cur_node = (obj *)(chunk + n);
		obj *next_node = nullptr;

		size_t index = FREELIST_INDEX(n);
		free_list[index] = cur_node;
		//将free_list各个节点串起来
		for (int i = 1; i < nobjs; i++) {
			next_node = (obj *)((char *)cur_node + n);
			cur_node->next = next_node;
			cur_node = next_node;
		}
		cur_node->next = nullptr;

		return result;
	}

	//实现内存池
	char *alloc::chunk_alloc(size_t size, int &nobjs) {

	}
}