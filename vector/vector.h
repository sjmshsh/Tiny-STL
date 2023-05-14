#pragma once
#include <assert.h>

namespace lxy
{
	template<class T>
	class vector
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		// 默认成员函数
		vector() // 构造函数
			:_start(nullptr)
			,_finish(nullptr)
			,_endofstorage(nullptr)
		{}


		vector(size_t n, const T& val) // 构造函数
			:_start(nullptr)
			, _finish(nullptr)
			, _endofstorage(nullptr) 
		{
			// 调用reserve函数将容器容量设置为n
			reserve(n);
			for (size_t i = 0; i < n; ++i)
			{
				push_back(val);
			}
		}
		
		
		template<class InputIterator>
		vector(InputIterator first, InputIterator last)
			:_start(nullptr)
			,_finish(nullptr)
			,_endofstorage(nullptr)
		{
			// 将迭代器区间在[first, last)的数据一个个尾插到容器当中
			while (first != last)
			{
				push_back(*first);
				first++;
			}
		}


		vector(const vector<T>& v) // 拷贝构造函数
			:_start(nullptr)
			, _finish(nullptr)
			, _endofstorage(nullptr)
		{
			// 调用reserve函数将容器容量设置与v相同
			reserve(v.capacity());
			// 将容器v一个一个的尾插过来
			for (auto e : v)
			{
				push_back(e);
			}
		}

		// 编译器接收右值的时候自动调用其拷贝构造函数
		vector<T>& operator=(const vector<T> v)// 赋值运算符重载
		{
			// 交互这两个对象
			swap(v);
			// 支持连续赋值
			return *this;
		}


		~vector()
		{
			// 避免对空指针进行释放
			if (_start)
			{
				delete[] _start;
				_start = nullptr;
				_finish = nullptr;
				_endofstorage = nullptr;
			}
		}


		// 迭代器相关函数
		iterator begin()
		{
			return _start;
		}

		iterator end()
		{
			return _finish;
		}

		const_iterator begin()const
		{
			return _start;
		}


		const_iterator end()const
		{
			return _finish;
		}

		// 容量和大小相关函数
		size_t size()const
		{
			return _finish - _start;
		}


		size_t capacity()const
		{
			return _endofstorage - _start;
		}


		void reserve(size_t n)
		{
			// 判断是否需要进行操作
			if (n > capacity())
			{
				// 记录当前容器当中有效数据的个数
				size_t sz = size();
				// 开辟一块可以容纳n个数据的空间
				T* tmp = new T[n];
				// 判断是否是空容器
				if (_start)
				{
					// 将容器当中的数据一个个拷贝到tmp当中
					for (size_t i = 0; i < sz; ++i)
					{
						tmp[i] = _start[i];
					}
					// 将容器本身存储数据的空间释放
					delete[] _start;
				}
				// 将tmp所维护的数据交给_start进行维护
				_start = tmp;
				// 容器有效数据的尾
				_finish = _start + sz;
				// 整个容器的尾
				_endofstorage = _start + n;
			}
		}


		void resize(size_t n, const T& val = T())
		{
			// 当n小于当前的size时
			if (n < size())
			{
				// 将size缩小到n
				_finish = _start + n;
			}
			else
			{
				// 当n大于当前的size的时候
				if (n > capacity())
				{
					reserve(n);
				}
				// 将size扩大到n
				while (_finish < _start + n)
				{
					*_finish = val;
					_finish++;
				}
			}
		}


		bool empty()const
		{
			return _start == _finish;
		}

		// 修改容器内部相关函数
		void push_back(const T& x)
		{
			// 判断是否需要扩容
			if (_finish == _endofstorage)
			{
				size_t newcapacity = capacity() == 0 ? 4 : 2 * capacity();
				reserve(newcapacity);
			}
			*_finish = x;
			_finish++; // _finish指针后移
		}


		void pop_back()
		{
			assert(!empty());
			_finish--;
		}


		// 如果需要增容，则需要在增容前记录pos与_start之间的间隔，然后通过该间隔确定在增容后
		// 的容器当中pos的指向，否则pos还指向原来被释放的空间
		void insert(iterator pos, const T& x)
		{
			if (_finish == _endofstorage)
			{
				// 记录pos与_start之间的间隔
				size_t len = pos - _start;
				size_t newcapacity = capacity() == 0 ? 4 : 2 * capacity();
				reserve(newcapacity);
				// 通过len找到pos在增容后容器当中的位置
				pos = _start + len;
			}
			// 将pos位置及其之后的数据统一向后挪动一位，以留出pos位置进行插入
			iterator end = _finish;
			while (end >= pos + 1)
			{
				*end = *(end - 1);
				end--;
			}
			// 将数据插入到pos位置
			*pos = x;
			// 数据个数增加一个，_finish后移
			_finish++;
		}

		iterator erase(iterator pos)
		{
			assert(!empty());
			// 将pos位置之后的数据统一向前移动一位，以覆盖pos位置的数据
			iterator it = pos + 1;
			while (it != _finish)
			{
				*(it - 1) = *it;
			}
			_finish--;
			return pos;
		}


		void swap(vector<T>& v);
		{
			::swap(_start, v._start);
			::swap(_finish, v._finish);
			::swap(_endofstorage, v._endofstorage);
		}

		// 访问容器相关的函数
		T& operator[](size_t i)
		{
			assert(i < size());
			return _start[i];
		}


		const T& operator[](size_t i)const
		{
			assert(i < size());
			return _start[i];
		}

	private:
		iterator _start; // 指向容器的头
		iterator _finish; // 指向有效数据的尾
		iterator _endofstorage; // 指向容器尾
	};
}
