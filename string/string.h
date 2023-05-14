#pragma once
#include<iostream>
#include <istream>
#include <ostream>

using namespace std;

namespace lxy
{
	// 模拟实现string类
	class string
	{
	public:
		// 迭代器
		typedef char* iterator;
		typedef const char* const_iterator;

		// 默认成员函数
		// 构造函数
		string(const char* str = "");
		// 拷贝构造函数
		string(const string& s);
		// 赋值运算符重载函数
		string& operator=(const string s);
		// 析构函数
		~string();

		// 迭代器相关函数
		iterator begin();
		iterator end();
		const_iterator begin()const;
		const_iterator end()const;

		// 容量和大小相关的函数
		size_t size()const;
		size_t capacity()const;
		void reserve(size_t n);
		void resize(size_t n, char ch = '\0');
		bool empty()const;

		// 修改字符串相关的函数
		void push_back(char ch);
		void append(const char* str);
		string& operator+=(const char* str);
		string& operator+=(char ch);
		string& insert(size_t pos, char ch);
		string& insert(size_t pos, const char* str);
		string& erase(size_t pos, size_t len);
		void clear();
		void swap(string& s);
		const char* c_str()const;

		// 访问字符串相关的函数
		char& operator[](size_t i);
		const char& operator[](size_t i)const;
		size_t find(char ch, size_t pos = 0)const;
		size_t find(const char *str, size_t pos = 0)const;

		// 关系运算符重载函数
		bool operator>(const string& s)const;
		bool operator>=(const string& s)const;
		bool operator<(const string& s)const;
		bool operator<=(const string& s)const;
		bool operator==(const string& s)const;
		bool operator!=(const string& s)const;

	private:
		char* _str; // 存储字符串
		size_t _size; // 记录字符串当前的有效长度
		size_t _capacity; // 记录字符串当前的容量
		static const size_t npos; // 静态成员变量(整形最大值)
	};
	const size_t string::npos = -1;

	// >>和<< 运算符重载
	istream& operator>>(istream& in, string& s);
	ostream& operator<<(ostream& out, const string& s);
	istream& getline(istream& in, string& s);

}
