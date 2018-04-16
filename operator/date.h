#pragma once 

#include <iostream>
#include <assert.h>

using namespace std;

class Date
{
public:
	Date(int year = 1900, int month = 1, int day = 1)
		:_year(year)
		, _month(month)
		, _day(day)
	{
		if (!IsInvalid())
		{
			assert(false);
		}
	}

	Date(const Date& d)		//��������
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}

	~Date(){}

	bool IsInvalid();
	void Show();

	Date& operator=(const Date& d);   //��ֵ���������

	bool operator==(const Date& d);	//==����
	bool operator!=(const Date& d);	//!=����

	bool operator>=(const Date& d);	//>=����
	bool operator<=(const Date& d);	//<=����

	// d1 < d2
	bool operator>(const Date& d);		//>����
	// d1 > d2 
	bool operator<(const Date& d);		//<����

	// d1 + 10 
	Date operator+(int day);			//+����
	Date& operator+=(int day);		//+=����

	Date operator-(int day);			//-����
	Date& operator-=(int day);		//-=����

	//++d1 
	Date& operator++();				//ǰ��++ 
	//d1++ 
	Date operator++(int);				//����++

	Date& operator--();				//ǰ��-- 
	Date operator--(int);				//����--

	int operator-(const Date& d);		//�������������������


	
private:
	int _year;
	int _month;
	int _day;
};


