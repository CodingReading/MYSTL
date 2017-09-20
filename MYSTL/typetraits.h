#ifndef TYPE_TRAITS_H_
#define TYPE_TRAITS_H_

/***********************************************
	type_traits.h
	允许针对不同的型别属性，在编译期完成函数派送
	根据型别是否具有non_trivial default ctor? non_trivial copy ctor?
	non_trivial assignment operator? non_trivial dtor? 如果不具备，我们在对
	这些型别进行复制，析构，拷贝，赋值时，不用调用constructor,destructor,而
	采用内存直接操作函数malloc(),memcpy()等
*************************************************/


/*
	POD 类型 ：Specifies that the type is POD (Plain Old Data) type. 
	This means the type is compatible with the types used in the C programming language,
	can be manipulated using C library functions: it can be created with std::malloc, 
	it can be copied with std::memmove, etc, and can be exchanged with C libraries directly, 
	in its binary form.			

	Requirements
		Either:
			a scalar type;
			a class type (class or struct or union) that is:
			an aggregate type;
			has no non-static members that are non-POD;
			has no members of reference type;
			has no user-defined copy constructor;
			has no user-defined destructor;
		(until C++11)
			a trivial type;
			a standard layout type;
			has no non-static members that are non-POD;
		(since C++11)
			an array of such type.
											//摘自cpp reference
*/

namespace mySTL {
	//编译器只有面对class objec形式的参数才会进行参数推导
	struct __true_type {};
	struct __false_type {};

	template <class type>
	struct __type_traits {
		typedef __true_type this_dummy_member_must_be_first;

		//做保守估计，默认型别都没有
		typedef __false_type has_trivial_default_constuctor;
		typedef __false_type has_trivial_copy_constructor;
		typedef __false_type has_trivial_assignment_operator;
		typedef __false_type has_trivial_destructor;
		typedef __false_type is_POD_type;
	};

	//以下针对C++内置类型做特化
	template <>
	struct __type_traits<bool> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<char> {
		typedef __true_type has_trivial_default_constructsor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<unsigned char> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<short> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<unsigned short> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<int> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<unsigned int> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<long> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<unsigned long> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<long long> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<unsigned long long> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<float> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<double> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template <>
	struct __type_traits<long double> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	//针对原生指针做偏特化
	template <class T>
	struct __type_traits<T*> {
		typedef __true_type has_trivial_default_constructor;
		typedef __true_type has_trivial_copy_constructor;
		typedef __true_type has_trivial_assignment_operatpr;
		typedef __true_type has_trivial_destructor;
		typedef __true_type is_POD_type;
	};

	template<class T>
	struct __type_traits<const T*>
	{
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};
}

#endif