#include "vector.h"

namespace mySTL {
	//构造函数使用
	template <class T, class Alloc = alloc>
	void vector<T, Alloc>::fill_initialize(const T& value, size_type n) {
		start = allocate_and_fill(value, n);
		finish = start + n;
		end_of_storage = finish;
	}

//=======================构造，析构相关函数===============================
	//explicit 抑制构造函数的隐式转换
	template <class T, class Alloc = alloc>
	explicit vector<T, Alloc>::vector(size_type n) {
		fill_initialize(n, T());
	}

	template <class T, class Alloc = alloc>
	vector<T, Alloc>::vector(size_type n, const T& value) {
		fill_initialize(n, value);
	}

	template <class T, class Alloc = alloc>
	template <class InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		allocate_and_copy(last - first, first, last);
	}

	template <class T, class Alloc = alloc>
	vector<T, Alloc>& 
	vector<T, Alloc>::operator = (const vector<T, Alloc>& v) {
		if (&v != this) {
			size_type v_size = v.size();
			if (v_size > capacity()) {
				iterator old_start = start;
				iterator old_finish = finish;
				allocate_and_copy(v_size, v.begin(), v.end());
				destroy_and_deallocate();
			}
			else if (v_size <= size()) {
				iterator t = copy(v.begin(), v.end(), start);
				destroy(t, finish);
			}
			else {
				copy(v.begin(), v.begin() + size(), start);
				finish = uninitialized_copy(v.begin() + size(), v.end(), finish);
			}
		}
		
		return *this;
	}

	template <class T, class Alloc = alloc>
	vector<T, Alloc>::~vector() {
		destroy_and_deallocate();
	}

//==========================容器操作相关============================
	template <class T, class Alloc = alloc>
	void vector<T, Alloc>::reserve(size_type n) {
		if (n < capacity())
			return;

		iterator new_start = date_allocator::allocate(n);
		iterator new_finish = uninitialized_copy(start, finish, new_start);

		destroy_and_deallocate();
		start = new_start;
		finish = new_finish;
		end_of_storage = start + n;
	}

	//释放掉未使用的内存
	template <class T, class Alloc = alloc>
	void vector<T, Alloc>::shink_to_fit() {
		destroy(finish, end_of_storage);
		date_allocator::deallocate(finish, end_of_storage - finish);
		--finish;
		--end_of_storage;
	}

	template <class T, class Alloc = alloc>
	void vector<T, Alloc>::push_back(const T& x) {
		insert(finish, x);
	}

	template <class T, class Alloc = alloc>
	void vector<T, Alloc>::pop_back() {
		--finish;
		destroy(finish);
	}

	template <class T, class Alloc = alloc>
	typename vector<T, Alloc>::iterator 
	vector<T, Alloc>::erase(iterator position) {
		if (position + 1 != finish)
			copy(position + 1, finish, position);
		--finish;
		destroy(finish);
		return position;
	}
	
	template <class T, class Alloc = alloc>
	typename vector<T, Alloc>::iterator
	vector<T, Alloc>::erase(iterator first, iterator last) {
		iterator i = copy(last, finish, first);
		destroy(i, finish);
		finish = finish - (last - first);
		return first;
	}

	template <class T, class Alloc = alloc>
	typename vector<T, Alloc>::iterator
	vector<T, Alloc>::resize(size_type new_size, const T& x) {
		if (new_size < size())
			erase(start + new_size, finish);
		else
			insert(finish, new_size - size(), x);
	}

	template <class T, class Alloc = alloc>
	typename vector<T, Alloc>::iterator
	vector<T, Alloc>::resize(size_type new_size) {
		resize(new_size, T());
	}

	template <class T, class Alloc = alloc>
	void vector<T, Alloc>::clear() {
		erase(start, finish); 
	}

	//未做异常处理
	template <class T, class Alloc = alloc>
	typename vector<T, Alloc>::iterator
	vector<T, Alloc>::insert(const_iterator pos, size_type n, const T& value) {
		size_type length = pos - start;
		if (n) {
			if (size_type(end_of_storage - finish) >= n) {
				//备用空间大于新增元素个数
				const size_type elems_after = finish - pos;
				iterator old_finish = finish;
				if (elems_after > n) {
					//插入点后的元素个数大于插入个数
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(pos, old_finish - n, old_finish);
					fill(pos, pos + n, value);
				}
				else {
					//插入点后的元素个数 小于等于 插入个数
					uninitialized_fill_n(finish, n - elems_after, value);
					finish += n - elems_after;
					uninitialized_copy(pos, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, value);
				}
			}
			else {
				//备用空间小于新增元素 
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);
				iterator new_start = date_allocator::allocate(len);
				//填充新空间
				iterator new_finish = uninitialized_copy(start, pos, new_start);
				new_finish = uninitialized_fill_n(new_finish, finish - pos, value);
				new_finish = uninitialized_copy(pos, finish, new_finish);
				//析构，并回收内存原来的空间
				destroy_and_deallocate();
				//重新赋值空间地址
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;

			}
			return start + length;
		}
		else
			return pos;
	}

	template <class T, class Alloc = alloc> 
	typename vector<T, Alloc>::iterator
	vector<T, Alloc>::insert(const_iterator pos, const T& value) {
		return insert(pos, 1, value);	//	转调用
	}

//========================比较相关============================
	template <class T, class Alloc>
	friend bool operator == (const vector<T, Alloc>& v1,
		const vector<T, Alloc>& v2) {
		if (v1.size() != v2.size())
			return false;
		auto t1 = v1.begin();
		auto t2 = v2.begin();
		while (t1 != v1.end()) {
			if (*t1 != *t2)
				return false;
			t1++;
			t2++;
		}
		return true;
	}

	template <class T, class Alloc>
	friend bool operator != (const vector<T, Alloc>& v1,
		const vector<T, Alloc>& v2) {
		return !(v1 == v2);
	}

	template <class T, class Alloc>
	friend bool operator > (const vector<T, Alloc>& v1,
		const vector<T, Alloc>& v2) {
		auto v1_start = v1.begin();
		auto v1_end = v1.end();
		auto v2_start = v2.begin();
		auto v2_end = v2.end();
		for (; (v1_start != v1_end) && (v2_start != v2_end);
					v1_start++, v2_start++) {
			if (*v1_start > *v2_start)
				return true;
			if (*v1_start < *v2_start)
				return false;
		}
		return (v1_start != v1_end) && (v2_start == v2_end);
	}

	template <class T, class Alloc>
	friend bool operator <= (const vector<T, Alloc>& v1,
		const vector<T, Alloc>& v2) {
		return (!v1 > v2);
	}

	template <class T, class Alloc>
	friend bool operator < (const vector<T, Alloc>& v1,
		const vector<T, Alloc>& v2) {
		auto v1_start = v1.begin();
		auto v1_end = v1.end();
		auto v2_start = v2.begin();
		auto v2_end = v2.end();
		for (; (v1_start != v1_end) && (v2_start != v2_end);
		v1_start++, v2_start++) {
			if (*v1_start < *v2_start)
				return true;
			if (*v1_start > *v2_start)
				return false;
		}
		return (v1_start == v1_end) && (v2_start != v2_end);
	}

	template <class T, class Alloc>
	friend bool operator >= (const vector<T, Alloc>& v1,
		const vector<T, Alloc>& v2) {
		return !(v1 < v2);
	}

//=========================空间分配，释放相关============================
	template <class T, class Alloc = alloc>
	void vector<T, Alloc>::destroy_and_deallocate() {
		if (capacity()) {
			destroy(start, finish);
			date_allocator::deallocate(start, end_of_storage - start);
		}
	}

	template <class T, class Alloc = alloc>
	typename vector<T, Alloc>::iterator
	vector<T, Alloc>::allocate_and_fill(size_type n, const T& x) {
		//配置n个元素空间
		iterator result = date_allocator::allocate(n);
		uninitialized_fill_n(result, n, x);
		return result;
	}

	template <class T, class Alloc = alloc>
	typename vector<T, Alloc>::iterator
	vector<T, Alloc>::allocate_and_copy(size_type n ,iterator first, iterator last) {
		start = Alloc::allocate(n);
		finish = uninitialized_copy(first, last, start);
		end_of_storage = start + n;
		return start;
	}
}
