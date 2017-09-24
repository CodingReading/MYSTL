#ifndef ALGORITHM_H_
#define ALGORITHM_H_

namespace mySTL {
	template <class T>
	T max(const T& a, const T& b) {
		return (a > b) ? a : b;
	}

	template <class T>
	void swap(T& a, T& b) {
		T t = a;
		a = b;
		b = t;
	}

	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}
}


#endif