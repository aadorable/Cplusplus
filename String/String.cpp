#define _CRT_SECURE_NO_WARNINGS 1

#include "String.h"
#include <windows.h>
#include <assert.h>

void String::Expand(size_t n)    //扩容
{
	if (n > _capacity)
	{
		_str = (char*)realloc(_str, n + 1);
		_capacity = n + 1;
	}
}

//增删查改
void String::PushBack(char ch)
{
	if (_size >= _capacity - 1)
	{
		Expand(_capacity * 2);
	}
	_str[_size] = ch;
	_size++;
	_str[_size] = '\0';    //最后加上'\0'
}

void String::PushBack(const char* str)
{
	size_t len = strlen(str);
	if (len + _size >= _capacity - 1)
	{
		Expand(len + _size);
	}
	strcpy(_str + _size, str);
	_size = strlen(_str);
}

void String::PopBack()
{
	if (_size > 0)
	{
		_str[_size - 1] = _str[_size];    //将'\0'前移
		--_size;
	}
}

void String::Insert(size_t pos, char ch)
{
	if (_size >= _capacity)
	{
		Expand(_capacity * 2);
	}
	size_t end = _size;
	while (end >= pos)
	{
		_str[end + 1] = _str[end];
		--end;
	}
	_str[pos] = ch;
	++_size;
}

void String::Insert(size_t pos, const char* str)
{
	size_t len = strlen(str);
	size_t tmp = len;
	if (_size + len > _capacity)
	{
		Expand(_size + len);
	}
	size_t end = _size;
	while (end >= pos)
	{
		_str[end + len] = _str[end];
		--end;
	}
	while (len-- > 0)
	{
		_str[pos++] = *str++;
	}
	_size += tmp;
}

void String::Erase(size_t pos, size_t n)
{
	if (pos + n >= _size - 1)
	{
		_str[pos] = '\0';
		_size = pos;
	}
	else
	{
		strcpy(_str + pos, _str + pos + n);     //相当于直接将'\0'拷到pos处
		_size -= n;
	}
}

size_t String::Find(char ch) const
{
	for (size_t i = 0; i < _size; ++i)
	{
		if (_str[i] == ch)
		{
			return i;
		}
	}
	return -1;
}

size_t String::Find(const char* str) const
{
	assert(str);
	char* start = (char*)_str;
	char* substart = (char*)str;
	char* ret = (char*)_str;
	while (*ret)
	{
		start = ret;
		if (*start && *substart && (*start == *substart))
		{
			++start;
			++substart;
		}
		if (*substart == '\0')
		{
			return 1;
		}
		substart = (char*)str;
		ret++;
	}
	return -1;
}

// s1.Swap(s2); 
void String::Swap(String& s)
{
	if (*this != s)
	{
		swap(_str, s._str);
		swap(_size, s._size);
		swap(_capacity, s._capacity);
	}
}

// s1 = s2 (现代写法)
String& String::operator=(String s)
{
	if (this != &s)
	{
		_size = s._size;
		_capacity = s._capacity;
		swap(_str, s._str);
	}
	return *this;
}

//// s1 = s2 (传统写法)
//String& String::operator=(String s)
//{
//	if (this != &s)
//	{
//		int len = strlen(s._str);
//		char* tmp = new char[len + 1];
//		strcpy(tmp, s._str);
//		_str = NULL;
//		_str = tmp;
//	}
//	return *this;
//

String String::operator+(const char* str)
{
	String *s = new String;
	s->_size = _size + strlen(str);
	s->_capacity = _capacity + strlen(str);
	s->_str = new char[s->_size + 1];
	strcpy(s->_str, _str);
	strcat(s->_str, str);
	return *s;
}

String& String::operator+=(const char* str)
{
	_size += strlen(str);
	_capacity += strlen(str);
	char *s = new char[_size + 1];
	strcpy(s, _str);
	strcat(s, str);
	delete[] _str;
	_str = s;
	return *this;
}

bool String::operator<(const String& s) const
{
	size_t i = 0;
	for (; i < _size && i < s._size; ++i)
	{
		if (_str[i] < s._str[i])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (i == _size)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool String::operator<=(const String& s) const
{
	return *this < s || *this == s;
}

bool String::operator>(const String& s) const
{
	return !(*this <= s);
}

bool String::operator>=(const String& s) const
{
	return !(*this < s);
}

bool String::operator==(const String& s) const
{
	size_t i = 0;
	for (; i < _size && i < s._size; ++i)
	{
		if (_str[i] != s._str[i])
		{
			return false;
		}
	}
	if (i == _size && i == s._size)
	{
		return true;
	}
	return false;
}

bool String::operator!=(const String& s) const
{
	return !(*this == s);
}

int main()
{
	//String s1("hello");
	//String s2(s1);
	//String s3("world!");
	//cout << s1;
	//cout << s2;
	//cout << s3;

	//String s4("HELLO");
	//s3 = s4;
	//cout << s3;
	//cout << s4;

	//printf("s1 == s2:%d", s1==s2);
	//printf("\n");
	//printf("s3 > s1:%d", s3>s1);
	//printf("\n");

	//s1.Swap(s3);
	//cout << s1;
	//cout << s3;

	//printf("s1 < s2:%d", s1< s2);
	//printf("\n");

	//String s1("hello");
	////s1 += "world";
	////cout << s1;
	//String s2;
	//s2 = s1 + " world";
	//cout << s1;
	//cout << s2;

	String s1("hello");
	s1.PushBack('w');
	s1.PushBack('o');
	s1.PushBack('r');
	s1.PushBack('l');
	s1.PushBack('d');
	cout << s1;
	s1.PushBack(" hahaha");
	cout << s1;
	s1.PopBack();
	s1.PopBack();
	s1.PopBack();
	s1.PopBack();
	cout << s1;
	s1.Insert(5, ' ');
	s1.Insert(11, '!');
	cout << s1;
	s1.Insert(13, "everyone ");
	cout << s1;
	s1.Erase(13, 9);
	cout << s1;
	size_t tmp = s1.Find('o');
	printf("%lu\n", tmp);
	tmp = s1.Find("hehe");
	printf("%lu\n", tmp);


	system("pause");
	return 0;
}