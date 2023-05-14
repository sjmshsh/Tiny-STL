#include "string.h"
#include <assert.h>

lxy::string::string(const char* str = "")
{
	_size = strlen(str);
	_capacity = _size;
	_str = new char[_capacity + 1];
	strcpy(_str, str); // 把C字符串拷贝到已经开好的空间
}

lxy::string::string(const string& s)
	:_str(nullptr)
	, _size(0)
	, _capacity(0)
{
	// 调用构造函数，构造出一个C字符串的s._str的对象
	string tmp(s._str);
	// 交换这两个对象
	swap(tmp);
}

lxy::string& lxy::string::operator=(string s)
{
	swap(s);
	return *this;
}

// 返回C类型的字符串
const char* lxy::string::c_str()const
{
	return _str;
}

lxy::string::~string()
{
	// 释放_str指向的空间
	delete[] _str;
	// 及时置空，防止非法访问
	_size = 0;
	_capacity = 0;
	_str = nullptr;
}

// 交换两个对象的数据
void lxy::string::swap(string& s)
{
	// 调用库里面的swap
	::swap(_str, s._str); // 交换两个对象的C字符串
	::swap(_size, s._size); // 交换两个对象的大小
	::swap(_capacity, s._capacity); // 交换两个对象的容量
}

lxy::string::iterator lxy::string::begin()
{
	return _str;
}

lxy::string::const_iterator lxy::string::begin()const
{
	return _str;
}

lxy::string::iterator lxy::string::end()
{
	return _str + _size;
}

lxy::string::const_iterator lxy::string::end()const
{
	return _str + _size;
}

size_t lxy::string::size()const
{
	return _size;
}

size_t lxy::string::capacity()const
{
	return _capacity;
}

bool lxy::string::empty()const
{
	return strcmp(_str, "") == 0;
}

// 改变容量 大小不变
void lxy::string::reserve(size_t n)
{
	// 当n大于对象当前容量时才能执行操作
	if (n > _capacity)
	{
		// 多开一个空间用来存放'\0'
		char* tmp = new char[n + 1];
		// 将对象原本的C字符串拷贝过来，包括('\0')
		strncpy(tmp, _str, _size + 1);
		delete[] _str;
		_str = tmp;
		_capacity = n;
	}
}

void lxy::string::resize(size_t n, char ch = '\0')
{
	// n 小于当前size
	if (n <= _size)
	{
		_size = n;
		_str[_size] = '\0';
	}
	else
	{
		if (n > _capacity)
		{
			reserve(n);
		}
		for (size_t i = _size; i < n; ++i)
		{
			_str[i] = ch;
		}
		_size = n;
		_str[_size] = '\0';
	}
}

// 尾插字符
void lxy::string::push_back(char ch)
{
	insert(_size, ch);
}

// 在pod位置插入字符
lxy::string& lxy::string::insert(size_t pos, char ch)
{
	// 检查下标的合法性
	assert(pos <= _size);
	// 判断是否需要扩容
	if (_size == _capacity)
	{
		size_t capacity = _capacity == 0 ? 4 : _capacity * 2;
		reserve(capacity);
	}
	// end指针指向的是字符串的末尾
	char* end = _str + _size;
	// 将pod位置及其之后的字符串向后挪动一位
	while (end >= _str + pos)
	{
		*(end + 1) = *(end);
		end--;
	}
	// pos位置上指定字符
	_str[pos] = ch;
	_size++;
	return *this;
}

lxy::string& lxy::string::insert(size_t pos, const char* str)
{
	assert(pos <= _size);
	// 计算需要插入的字符串长度（不包含'\0'）
	size_t len = strlen(str);
	if (len + _size > _capacity)
	{
		reserve(len + _size);
	}
	char* end = _str + _size;
	while (end >= _str + pos)
	{
		*(end + len) = *(end);
		end--;
	}
	strncpy(_str + pos, str, len);
	_size += len;
	return *this;
}

void lxy::string::append(const char* str)
{
	insert(_size, str);
}

lxy::string& lxy::string::operator+=(const char* str)
{
	append(str);
	return *this;
}

lxy::string& lxy::string::operator+=(char ch)
{
	insert(_size, ch);
	return *this;
}

// 清空字符串
void lxy::string::clear()
{
	_size = 0;
	// 字符串后面放上'\0'
	_str[_size] = '\0';
}

lxy::string& lxy::string::erase(size_t pos, size_t len = npos)
{
	assert(pos < _size);
	// pos位置及其后面的有效字符总数
	size_t n = _size - pos;
	// 说明pos位置及其后面的字符都被删除
	if (len >= n)
	{
		_size = pos;
		_str[_size] = '\0';
	}
	else // 说明pos位置及其后面的有效字符需要保留一部分
	{
		// 用需要保留的有效字符覆盖需要删除的有效字符
		strcpy(_str + pos, _str + pos + len);
		_size -= len;
	}
	return *this;
}

// []运算符重载(可读可写)
char& lxy::string::operator[](size_t i)
{
	// 检查下标的合法性
	assert(i < _size);
	return _str[i];
}


const char& lxy::string::operator[](size_t i)const
{
	assert(i < _size);
	return _str[i];
}

bool lxy::string::operator>(const string& s)const
{
	return strcmp(_str, s._str) > 0;
}

bool lxy::string::operator==(const string& s)const
{
	return strcmp(_str, s._str) == 0;
}

bool lxy::string::operator>=(const string& s)const
{
	return (*this > s) || (*this == s);
}

bool lxy::string::operator<(const string& s)const
{
	return !(*this >= s);
}

bool lxy::string::operator<=(const string& s)const
{
	return !(*this > s);
}

bool lxy::string::operator!=(const string& s)const
{
	return !(*this == s);
}


istream& lxy::operator>>(istream& in, string& s)
{
	// 清空字符串
	s.clear();
	// 读取一个字符
	char ch = in.get();
	// 当读取到的字符不是空格或者'\n'的时候继续读取
	while (ch != ' ' && ch != '\n')
	{
		// 将读取到的字符尾插到字符串后面
		s += ch;
		// 继续读取字符
		ch = in.get();
	}
	// 支持连续输入
	return in;
}

ostream& lxy::operator<<(ostream& out, const string& s)
{
	for (auto e : s)
	{
		cout << e;
	}
	return out;
}

//读取一行含有空格的字符串
istream& lxy::getline(istream& in, string& s)
{
	s.clear(); //清空字符串
	char ch = in.get(); //读取一个字符
	while (ch != '\n') //当读取到的字符不是'\n'的时候继续读取
	{
		s += ch; //将读取到的字符尾插到字符串后面
		ch = in.get(); //继续读取字符
	}
	return in;
}


size_t lxy::string::find(char ch, size_t pos = 0)const
{
	// 检测下标的合法性
	assert(pos < _size);
	// 从pos位置开始向后寻找目标字符
	for (size_t i = pos; i < _size; ++i)
	{
		if (_str[i] == ch)
		{
			// 找到目标字符，返回其下标
			return i;
		}
	}
	// 没有找到目标字符，返回npos
	return npos;
}


size_t lxy::string::find(const char* str, size_t pos = 0)const
{
	assert(pos < _size);
	const char* ret = strstr(_str + pos, str);
	if (ret)
	{
		// 返回字符串第一个字符的下标
		return ret - _str;
	}
	else
	{
		return npos;
	}
}
