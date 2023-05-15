#pragma once
#include <deque>

namespace lxy //防止命名冲突
{
	template<class T, class Container = std::deque<T>>
	class queue
	{
	public:
		//队尾入队列
		void push(const T& x)
		{
			_con.push_back(x);
		}
		//队头出队列
		void pop()
		{
			_con.pop_front();
		}
		//获取队头元素
		T& front()
		{
			return _con.front();
		}
		const T& front() const
		{
			return _con.front();
		}
		//获取队尾元素
		T& back()
		{
			return _con.back();
		}
		const T& back() const
		{
			return _con.back();
		}
		//获取队列中有效元素个数
		size_t size() const
		{
			return _con.size();
		}
		//判断队列是否为空
		bool empty() const
		{
			return _con.empty();
		}
		//交换两个队列中的数据
		void swap(queue<T, Container>& q)
		{
			_con.swap(q._con);
		}
	private:
		Container _con;
	};
}
