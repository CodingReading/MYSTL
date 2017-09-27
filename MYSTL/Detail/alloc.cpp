/*
* alloc.c
* 实现内存池，空间分配
*/

#include "../alloc.h"

namespace mySTL {
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
		//bug fix  for (int i = 1; i < nobjs; i++)
		//原来这样多连接了一个空间，两天时间定位到bug
		for (int i = 2; i < nobjs; i++) {
			next_node = (obj *)((char *)cur_node + n);
			cur_node->next = next_node;
			cur_node = next_node;
		}
		cur_node->next = nullptr;

		return result;
	}

	//实现内存池
	char *alloc::chunk_alloc(size_t size, int& nobjs) {
		char *result = 0;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes) {
			//内存池剩余空间满足需求
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else if (bytes_left >= size) {
			//内存池剩余空间不能满足需求量，但够一个以上区块
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else{
			//内存池空间一个区块也不够
			//理想分配的空间大小为 2 * total_bytes + n
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);

			//让内存池中的残余空间还有利用价值,将残余空间编入free_list
			if (bytes_left > 0) {
				size_t index = FREELIST_INDEX(bytes_left);
				((obj*)start_free)->next = free_list[index];
				free_list[index] = (obj*)start_free;
			}

			//向系统申请空间配置内存池
			start_free = (char*)malloc(bytes_to_get);
			if (start_free == 0) {
				//申请空间失败
				//如果free_list中有尚未利用的区块，且区块够大
				size_t index = 0;
				obj *p = NULL;
				for (size_t i = size; i <= MAX_BYTES; i += ALIGN) {
					index = FREELIST_INDEX(i);
					p = free_list[index];
					if (!p) {
						free_list[index] = p->next;
						start_free = (char*)p;
						end_free = start_free + i;
						//递归调用自己，修正nobjs
						return chunk_alloc(size, nobjs);
					}
				}
				//向系统申请空间失败，且free list中也没有未使用的比size大的区块
				end_free = 0;
			}

			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//递归调用自己，修正 nobjs,调整start_free
			return chunk_alloc(size, nobjs);
		}
	}
}