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

	Date(const Date& d)		//拷贝构造
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}

	~Date(){}

	bool IsInvalid();
	void Show();

	Date& operator=(const Date& d);   //赋值运算符重载

	bool operator==(const Date& d);	//==重载
	bool operator!=(const Date& d);	//!=重载

	bool operator>=(const Date& d);	//>=重载
	bool operator<=(const Date& d);	//<=重载

	// d1 < d2
	bool operator>(const Date& d);		//>重载
	// d1 > d2 
	bool operator<(const Date& d);		//<重载

	// d1 + 10 
	Date operator+(int day);			//+重载
	Date& operator+=(int day);		//+=重载

	Date operator-(int day);			//-重载
	Date& operator-=(int day);		//-=重载

	//++d1 
	Date& operator++();				//前置++ 
	//d1++ 
	Date operator++(int);				//后置++

	Date& operator--();				//前置-- 
	Date operator--(int);				//后置--

	int operator-(const Date& d);		//计算两个日期相差天数


	
private:
	int _year;
	int _month;
	int _day;
};


