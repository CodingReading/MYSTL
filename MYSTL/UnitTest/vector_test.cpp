#include "vector_test.h"

namespace mySTL {
	namespace vector_test {
		//测试构造析构
		void test_case1() {
			myVec<int> v1(10);
			myVec<int> v2(1000000, 8);
			myVec<int> v3(v2);
			myVec<int> v4(v3.begin(), v3.end());
			myVec<int> v5 = v4;

			assert(v1.size() == 10);
			assert(v4.size() == 1000000);
			v1.~vector();
			assert(v1.empty());
			mySTL::test::container_equal(v4, v5);
		}
		//容量相关
		void test_case2() {
			myVec<int> v1(10, 8);
			assert(v1.size() == 10);
			v1.reserve(20);
			assert(v1.capacity() == 20);
			v1.shrink_to_fit();
			assert(v1.size() == 10);
		}

		//容器操作相关
		void test_case3() {
			myVec<int> v1;
			for (int i = 0; i < 20; i++)
				v1.insert(v1.begin(), i);

			for (int i = 0; i < 10; i++)
				v1.pop_back();

			for (int i = 9; i >= 10; i--)
				v1.push_back(i);

			v1.resize(10);

			myVec<int> v2(10, 8);
			v2.swap(v1);
		}

		//大小比较相关
		void test_case4() {
			myVec<int> v1(10, 8);
			myVec<int> v2(v1);
			assert(v1 == v2);

			myVec<int> v3(11, 8);
			assert(v1 != v3);
			assert(v3 > v1);
			assert(v1 < v3);

			myVec<int> v4(10, 8);
			assert(v1 >= v4);
			assert(v1 <= v4);
		}

		void test_all() {
			test_case1();
			test_case2();
			test_case3();
			test_case4();
		}

	}
}