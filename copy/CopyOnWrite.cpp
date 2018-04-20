#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

//class String
//{
//public:
//	String(char* str = "")
//		:_str(new char[strlen(str) + 1])
//		, _pCount(new size_t(1))
//	{
//		strcpy(_str, str);
//	}
//	// s2(s1) 
//	String(const String& s)
//		:_str(s._str)
//		, _pCount(s._pCount)
//	{
//		(*_pCount)++;
//	}
//	//s2 = s1 
//	String& operator=(const String& s)
//	{
//		if (this != &s)
//		{
//			if (--(*_pCount) == 0)
//			{
//				delete[] _str;
//				delete _pCount;
//			}
//			_str = s._str;
//			_pCount = s._pCount;
//			(*_pCount)++;
//		}
//		return *this;
//	}
//	~String()
//	{
//		if (--(*_pCount) == 0)
//		{
//			delete[] _str;
//			delete _pCount;
//		}
//	}
//	const char* c_str()
//	{
//		return _str;
//	}
//	//�����ü����������������µ����ü���
//	void CopyOnWrite()
//	{
//		if ((*_pCount)-- > 1)
//		{
//			char* tmp = new char[strlen(_str) + 1];
//			strcpy(tmp, _str);
//			_str = tmp;
//			_pCount = new size_t(1);
//		}
//	}
//
//	//���ָ��ÿռ��ָ��ֻ��һ����ֱ�ӽ����޸�
//	//���ָ��ÿռ��ָ��������һ�������дʱ�������ٿռ�
//	char& operator[](size_t pos)
//	{
//		if (*_pCount > 1)
//		{
//			CopyOnWrite();
//		}
//		return _str[pos];
//	}
//private:
//	char* _str;
//	size_t* _pCount;
//};

class String
{
public:
	String(char* str = "")
		:_str(new char[strlen(str) + 4 + 1])
	{
		if (_str == NULL)
		{
			*(int*)_str = 1;
			*(_str + 4) = '\0';
		}
		else
		{
			*(int*)_str = 1;    //ǰ�ĸ��ֽڴ�����ü���
			_str += 4;
			strcpy(_str, str);
		}
	}
	int& GetCount()
	{
		return *((int*)(_str - 4));
	}
	// s2(s1) 
	String(const String& s)
		:_str(s._str)
	{
		++GetCount();     //��ǰƫ���ĸ��ֽڵ����ü���������������1
	}
	//s2 = s1 
	String& operator=(const String& s)
	{
		if (this != &s)
		{
			if (--GetCount() == 0)
			{
				delete[] (_str - 4);
				_str = NULL;
			}
			_str = s._str;    //ָ���¿ռ�
			++GetCount();    //���¿ռ�ļ�������1
		}
		return *this;
	}
	~String()
	{
		if (_str == NULL)
		{
			return;
		}
		else
		{
			if (--GetCount() == 0)
			{
				delete [] (_str - 4);
				_str = NULL;
			}
		}
	}
	const char* c_str()
	{
		return _str;
	}
	void CopyOnWrite()
	{
		if (GetCount()-- > 1)
		{
			char* tmp = new char[strlen(_str) + 4 + 1];
			tmp += 4;
			strcpy(tmp, _str);
			_str = tmp;
			GetCount() = 1;
		}
	}
	char& operator[](size_t pos)
	{
		if (GetCount() > 1)
		{
			CopyOnWrite();
		}
		return _str[pos];
	}
private:
	char* _str; // ���ü�����ͷ�� 
};

int main()
{
	String s1("abcdef");
	String s2(s1);
	String s3(s1);
	String s4("hello");
	s1 = s4;
	s2[0] = 'x';
	return 0;
}