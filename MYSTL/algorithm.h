#ifndef ALGORITHM_H_
#define ALGORITHM_H_

namespace mySTL {
	template <class T>
	T max(T& a, T& b) {
		return (a > b) ? a : b;
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