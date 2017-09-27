#ifndef LIST_IMPL_H_
#define LIST_IMPL_H_

namespace mySTL {
	template <class T, class Alloc>
	list<T, Alloc>::list(const list<T, Alloc>& other) {
		empty_initialize();
		//bug , 追踪到析构函数中的clear死循环
		for (link_type t = other.node->next; t != other.node; t = t->next);
	
			//std::cout << t->data << std::endl;
	}

	template <class T, class Alloc>
	list<T, Alloc>::~list() {
		clear();
		destroy_node(node);
	}

	template <class T, class Alloc>
	typename list<T, Alloc>::size_type
	list<T, Alloc>::size() const{
		size_type i = 0;
		iterator t = node;
		t++;
		for (; t != node; t++)
			i++;
		return i;
	}

	template <class T, class Alloc>
	void list<T, Alloc>::push_front(const T& x) {
		insert(begin(), x);
	}

	template <class T, class Alloc>
	void list<T, Alloc>::push_back(const T& x) {
		insert(end(), x);
	}

	template <class T, class Alloc>
	void list<T, Alloc>::pop_front() {
		erase(begin());
	}

	template <class T, class Alloc>
	void list<T, Alloc>::pop_back() {
		iterator t = end();
		erase(--t);
	}

	template <class T, class Alloc>
	void list<T, Alloc>::clear() {
		link_type cur = node->next;
		link_type t = nullptr;
		while (cur != node) {
			t = cur->next;
			destroy_node(cur);
			cur = t;
		}

		node->prev = node;
		node->next = node;
	}

	template <class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(iterator pos, const T& val) {
		link_type temp = create_node(val);
		//调整四个指针
		temp->next = pos.node;
		temp->prev = pos.node->prev;
		((pos.node)->prev)->next = temp;
		pos.node->prev = temp;
		return (iterator)temp;
	}

	template <class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::erase(iterator pos) {
		link_type next_node = pos.node->next;
		link_type prev_node = pos.node->prev;
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destroy_node(pos.node);
		return (iterator)next_node;
	}

	//将值为value的所有元素移除
	template <class T, class Alloc>
	void list<T, Alloc>::remove(const T& value) {
		iterator cur = begin();
		while (cur != end()) {
			if (*cur == value)
				cur = erase(cur);
			else
				cur++;
		}
	}

	template <class T, class Alloc>
	void list<T, Alloc>::splice(iterator pos, list<T, Alloc>& x) {
		if (!x.empty() && (&x != this))
			transfer(pos, x.begin(), x.end());
	}

	//已排好序的链表合并
	template <class T, class Alloc>
	void list<T, Alloc>::merge(list<T, Alloc>& x) {
		iterator first1 = begin();
		iterator end1 = end();
		iterator first2 = x.begin();
		iterator end2 = x.end();

		while (first1 != end1 && first2 != end2) {
			if (*first1 <= *first2)
				++first1;
			else {
				iterator t = first2++;
				transfer(first1, t, first2);
			}
		}

		if (first2 != end2)
			transfer(first1, first2, end2);
	}

	template <class T, class Alloc>
	void list<T, Alloc>::reverse() {
		//链表为空或仅有一个元素
		if (node->next == node || node->next->next == node)
			return;

		iterator cur = begin();
		++cur;
		while (cur != end()) {
			iterator t = cur++;
			transfer(begin(), t, cur);
		}
	}

	template <class T, class Alloc>
	void list<T, Alloc>::swap(list<T, Alloc>& x) {
		link_type t = node;
		node = x.node;
		x.node = t;
	}

	//采用归并排序算法
	template <class T, class Alloc>
	void list<T, Alloc>::sort() {
		if (node->next == node || node->next->next == node)
			return;
		list<T, Alloc> temp;
		list<T, Alloc> counter[64];
		int depth = 0;		//层次
		while (!empty()) {
			temp.transfer(temp.begin(), begin(), begin() + 1);	//取出一个节点
			int i = 0;
			while (i < depth && !counter[i].empty()) {
				//小于当前深度且counter[i]不为空
				counter[i].merge(temp);
				temp.swap(counter[i++]);
			}

			temp.swap(counter[i]);
			if (i == depth)
				depth++;
		}

		for (int i = 1; i < depth; i++)
			counter[i].merge(counter[i - 1]);
		swap(counter[depth - 1]);
	}


	template <class T, class Alloc>
	void list<T, Alloc>::transfer(iterator pos, iterator first, iterator last) {
		if (pos == last)
			return;
		last.node->prev->next = pos.node;
		first.node->prev->next = last.node;
		pos.node->prev->next = first.node;
		link_type t = pos.node->prev;
		pos.node->prev = last.node->prev;
		last.node->prev = first.node->prev;
		first.node->prev = t;
	}
}

#endif