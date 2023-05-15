#pragma once
#include <assert.h>
#include <iostream>

namespace lxy
{
	// 模拟实现list当中的节点类
	template<class T>
	struct _list_node
	{
		// 成员函数
		_list_node(const T& val = T()) // 构造函数
			: _val(val)
			, _prev(nullptr)
			, _next(nullptr)
		{}

		// 成员变量
		T _val; // 数据域
		_list_node<T>* _next; // 后继指针
		_list_node<T>* _prev; // 前驱指针
	};

	// 模拟实现list迭代器
	template<class T, class Ref, class Ptr>
	struct _list_iterator
	{
		typedef _list_node<T> node;
		// Ref 引用类型
		// Ptr 指针类型
		// 当我们使用普通迭代器时，编译器就会实例化出一个普通迭代器对象；当我们使用const迭代器时，编译器就会实例化出一个const迭代器对象。
		// 若该迭代器类不设计三个模板参数，那么就不能很好的区分普通迭代器和const迭代器。
		typedef _list_iterator<T, Ref, Ptr> self;

		// 构造函数
		_list_iterator(node* pnode)
			:_pnode(pnode)
		{}

		// 各种运算符重载函数
		self operator++() // 前置自增运算符的重载形式
		{
			_pnode = _pnode->_next;
			return *this; // 返回自增后的节点指针
		}

		self operator--()
		{
			_pnode = _pnode->_prev;
			return *this;
		}

		self operator++(int) // 后置自增运算符的重载形式
		{
			// 记录当前节点指针的指向
			self tmp(*this);
			_pnode = _pnode->_next;
			return tmp; // 返回自增前的节点指针
		}

		self operator--(int)
		{
			self tmp(*this);
			_pnode = _pnode->_prev;
			return tmp;
		}

		bool operator==(const self& s)const
		{
			return _pnode == s._pnode;
		}

		bool operator!=(const self& s)const
		{
			return _pnode != s._pnode;
		}

		Ref operator*()
		{
			return _pnode->_val;
		}

		Ptr operator->()
		{
			// 返回结点指针所指向的数据的地址
			return &_pnode->_val;
		}

		// 成员变量
		node* _pnode; // 一个指向节点的指针
	};

	// 模拟实现list
	template<class T>
	class list
	{
	public:
		typedef _list_node<T> node;
		typedef _list_iterator<T, T&, T*> iterator;
		typedef _list_iterator<T, const T&, const T*> const_iterator;

		// 默认成员函数
		list()
		{
			// 申请一个头节点
			_head = new node;
			// 头结点的后继指针指向自己
			_head->_next = _head;
			// 头结点的前驱指针指向自己
			_head->_prev = _head;
		}

		list(const list<T>& lt)
		{
			// 申请一个头结点
			_head = new node;
			_head->_next = _head;
			_head->_prev = _head;
			for (const auto& e : lt)
			{
				// 将容器lt当中的数据一个个尾插到新构造的容器后面
				push_back(e);
			}
		}

		// 现代写法
		// 编译器接受右值的时候自动调用其拷贝构造函数
		list<T>& operator=(const list<T> lt)
		{
			// 交换这两个对象
			swap(lt);
			// 支持连续赋值
			return *this;
		}

		~list()
		{
			// 清理容器
			clear();
			delete _head;
			_head = nullptr;
		}

		// 迭代器相关函数
		iterator begin()
		{
			// 使用头结点后一个结点的地址构造出来的普通迭代器
			return iterator(_head->_next);
		}

		iterator end()
		{
			return iterator(_head);
		}

		const_iterator begin() const
		{
			return const_iterator(_head->_next);
		}

		const_iterator end() const
		{
			return const_iterator(_head);
		}

		// 访问容器相关函数
		T& front()
		{
			return *begin();
		}

		T& back()
		{
			// 返回最后一个有效数据的引用
			return *(--end());
		}

		const T& front()const
		{
			return *begin();
		}

		const T& back()const
		{
			return *(--end());
		}

		// 插入，删除函数
		void insert(iterator pos, const T& x)
		{
			// 检查pos合法性
			assert(pos._pnode);

			// 迭代器pos处的结点指针
			node* cur = pos._pnode;
			// 迭代器pos前一个位置的结点指针
			node* prev = cur->_prev;
			// 根据所给数据x构造一个待插入结点
			node* newnode = new node(x);

			// 建立newnode与cur之间的双向关系
			newnode->_next = cur;
			cur->_prev = newnode;
			// 建立newnode与prev之间的双向关系
			newnode->_prev = prev;
			prev->_next = newnode;
		}

		iterator erase(iterator pos)
		{
			// 检查pos的合法性
			assert(pos._pnode);
			// 删除的结点不能是头结点
			assert(pos != end());

			// 迭代器pos处的结点指针
			node* cur = pos._pnode;
			// 迭代器pos前一个位置的结点指针
			node* prev = cur->_prev;
			// 迭代器pos后一个位置的结点指针
			node* next = cur->_next;

			delete cur;

			// 建立prev和next之间的双向关系
			prev->_next = next;
			next->_prev = prev;

			// 返回给迭代器pos的下一个迭代器
			return iterator(next);
		}

		void push_back(const T& x)
		{
			// 在头结点前插入结点
			insert(end(), x);
		}

		void pop_back()
		{
			// 删除头结点的前一个结点
			erase(--end());
		}

		void push_front(const T& x)
		{
			insert(begin(), x);
		}

		void pop_front()
		{
			erase(begin());
		}

		// 其他函数
		size_t size() const
		{
			size_t sz = 0;
			const_iterator it = begin();
			while (it != end())
			{
				sz++;
				it++:
			}
			return sz;
		}

		void resize(size_t n, const T& val = T())
		{
			// 获取第一个有效数据的迭代器
			iterator i = begin();
			// 记录当前所遍历的数据个数
			size_t len = 0;
			while (len < n && i != end())
			{
				len++;
				i++;
			}
			// 说明容器当中的有效数据个数大于或者等于n
			if (len == n)
			{
				// 只保留前n个有效数据
				while (i != end())
				{
					// 每次删除后接受下一个数据的迭代器
					i = erase(i);
				}
			}
			else
			{
				// 说明容器当中的有效数据个数小于n
				while (len < n)
				{
					// 尾插数据为val的结点，直到容器当中的有效数据个数为n
					push_back(val);
					len++;
				}
			}
		}
		
		// 用于情况容器
		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}

		bool empty() const
		{
			return begin() == end();
		}

		void swap(list<T>& lt)
		{
			// 交换两个容器的头指针即可
			::swap(_head, lt._head);
		}
	private:
		node* _head; // 指向链表头节点的指针
	};

}
