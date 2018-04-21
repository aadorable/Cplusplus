#pragma once

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

class String
{
	friend ostream& operator<<(ostream& _cout, const String& s);
	friend istream& operator>>(istream& _cin, String& s);

public:
	String(const char* str = "")
		:_str(new char[strlen(str) + 1])
		, _size(strlen(str))
		, _capacity(strlen(str) + 1)
	{
		strcpy(_str, str);
	}

	// String s2(s1) 
	String(const String& s)
		:_str(new char[strlen(s._str) + 1])
		, _size(strlen(s._str))
		, _capacity(strlen(s._str) + 1)
	{
		_str = s._str;
		_size = s._size;
		_capacity = s._capacity;
	}

	~String()
	{
		if (_str)
		{
			delete[] _str;
			_str = NULL;
		}
	}

	const char* c_str()
	{
		return _str;
	}

	//ÔöÉ¾²é¸Ä
	void Expand(size_t n);
	void PushBack(char ch);
	void PushBack(const char* str);
	void PopBack();
	void Insert(size_t pos, char ch);
	void Insert(size_t pos, const char* str);
	void Erase(size_t pos, size_t n);

	size_t Find(char ch) const;
	size_t Find(const char* str) const; 

	void Swap(String& s);
	// s1 = s2 
	String& operator=(String s);
	// s1 + 'a' 
	String operator+(char ch);
	String& operator+=(char ch);
	String operator+(const char* str);
	String& operator+=(const char* str);

	bool operator<(const String& s) const;
	bool operator<=(const String& s) const;
	bool operator>(const String& s) const;
	bool operator>=(const String& s) const;
	bool operator==(const String& s) const;
	bool operator!=(const String& s) const;

private:
	char* _str;
	size_t _size;
	size_t _capacity;
};

ostream& operator<<(ostream& _cout, const String& s)
{
	_cout << "str: " <<s._str << "  size:" << s._size << "  capacity:" << s._capacity << endl;
	return _cout;
}

istream& operator>>(istream& _cin, String& s)
{
	_cin >> s._str;
	return _cin;
}