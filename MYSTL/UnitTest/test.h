/*
 *   容器测试需要的函数
 */


#include <iostream>

namespace mySTL {
	namespace test {
		template <class Container1, class Container2>
		bool container_equal(Container1 c1, Container2 c2) {
			auto first1 = c1.begin();
			auto last1 = c1.end();
			auto first2 = c2.begin();
			auto last2 = c2.end();

			for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
				if (*first1 != *first2)
					return false;
			}
			return (first1 == last1) && (first2 == last2);
		}

		template <class Container>
		void container_print(Container c) {
			for (auto t : c)
				std::cout << t << " ";
			std::cout << std::endl;
		}
	}
}