#ifndef ITERATOR_H_
#define ITERATOR_H_

namespace mySTL {
	struct input_iterator_tag {};			//所指对象只读
	struct output_iterator_tag {};			//所指对象只写
	struct forward_iterator_tag : public input_iterator_tag {};	//区间上读写
	struct bidirectional_iterator_tag : public forward_iterator_tag {};	//双向
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	template<class Category, class T, class Distance = ptrdiff_t,
			 class Pointer = T*, class Reference = T&>
	struct iterator {
		typedef Category	iterator_category;
		typedef T			value_type;					//迭代器所指对象型别
		typedef Distance	difference_type;			//两个迭代器之间的距离
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	//萃取迭代器特性
	template<class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference 			reference;
	};

	//针对原生指针偏特化
	template <class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	//针对原生pointer to const 指针偏特化
	template <class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef	ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template <class T, class Distance = ptrdiff_t>
	struct input_iterator {
		typedef input_iterator_tag		iterator_category;
		typedef T						value_type;					
		typedef Distance				difference_type;			
		typedef T*						pointer;					
		typedef T&						reference;
	};

	template <class T, class Distance = ptrdiff_t>
	struct output_iterator {
		typedef output_iterator_tag		iterator_category;
		typedef void					value_type;
		typedef void					difference_type;
		typedef void					pointer;
		typedef void					reference;
	};

	template <class T, class Distance = ptrdiff_t>
	struct forward_iterator {
		typedef forward_iterator_tag	iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef T*						pointer;
		typedef T&						reference;
	};

	template <class T, class Distance = ptrdiff_t>
	struct bidirectional_iterator {
		typedef bidirectional_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef Distance						difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	};

	template <class T, class Distance = ptrdiff_t>
	struct random_access_iterator {
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef Distance						difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	};

	//获取迭代器类型
	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//获取迭代器difference_type;
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	difference_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//获取value type
	template <class Itreator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
}



#endif