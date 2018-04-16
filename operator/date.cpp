#define _CRT_SECURE_NO_WARNINGS 1

#include "date.h"

void Date::Show()
{
	cout << _year << "-" << _month << "-" << _day << endl;
}

bool IsLeapYear(int year)
{
	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
	{
		return true;
	}
	return false;
}

int GetMonthDay(int year, int month)
{
	int array[] = { -1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int day = array[month];
	if (month == 2 && IsLeapYear(year))
	{
		day = 29;
	}
	return day;
}

bool Date::IsInvalid()
{
	return _year >= 0
		&& _month >= 0 && _month <= 12
		&& _day > 0 && _day <= GetMonthDay(_year, _month);
}

Date& Date::operator=(const Date& d)
{
	if (this != &d)
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}
	return *this;
}


bool Date::operator==(const Date& d)		//隐含的this指针
{
	return this->_year == d._year
		&& this->_month == d._month
		&& this->_day == d._day;
}

bool Date::operator!=(const Date& d)
{
	return this->_year != d._year
		|| this->_month != d._month
		|| this->_day != d._day;
}

bool Date::operator>=(const Date& d)
{
	return !(*this < d);
}

bool Date::operator<=(const Date& d)
{
	return *this < d || *this == d;
}

bool Date::operator>(const Date& d)
{
	return !(*this <= d);
}

bool Date::operator<(const Date& d)
{
	if ((_year > d._year)
		|| (_year == d._year && _month > d._month)
		|| (_year == d._year && _month == d._month && _day > d._day))
	{
		return true;
	}
	return false;
}


Date Date::operator+(int day)		//本身并没有改变，不能返回引用
{
	Date ret(*this);
	ret._day += day;
	while (ret._day > GetMonthDay(ret._year, ret._month))
	{
		int monthday = GetMonthDay(ret._year, ret._month);
		ret._day -= monthday;
		ret._month++;
		if (ret._month > 12)
		{
			ret._month = 1;
			ret._year++;
		}
	}
	return ret;
}

Date& Date::operator+=(int day)
{
	*this = *this + day;
	return *this;
}

Date Date::operator-(int day)
{
	if (day < 0)
	{
		return *this + (-day);
	}
	Date ret(*this);
	ret._day -= day;
	while (ret._day < 0)
	{
		if (ret._month == 1)
		{
			ret._month = 12;
			ret._year--;
		}
		else
		{
			ret._month--;
		}
		int monthday = GetMonthDay(ret._year, ret._month);
		ret._day += monthday;
	}
	return ret;
}

Date& Date::operator-=(int day)
{
	*this = *this - day;
	return *this;
}

Date& Date::operator++() // 前置（效率高）
{
	*this += 1;
	return *this;
}

Date Date::operator++(int) // 后置 
{
	Date tmp(*this);
	tmp = *this + 1;
	return tmp;
}

Date& Date::operator--()
{
	*this -= 1;
	return *this;
}


Date Date::operator--(int)
{
	Date tmp(*this);
	tmp = *this - 1;
	return tmp;
}

int Date::operator-(const Date& d)		//计算两个日期相差天数
{
	//先确定哪个日期小，然后将小的往大的加，知道两个日期相等，就得到相差天数
	int flag = 1;
	Date max(*this);
	Date min(d);
	if (*this < d)
	{
		min = *this;
		max = d;
		flag = -1;
	}
	int days = 0;
	while (min < max)
	{
		min++;
		days++;
	}
	return days * flag;
}



int main()
{
	Date d1(2018, 3, 26);
	d1.Show();
	Date d2(2018, 4, 25);
	d2 = d1;
	d2.Show();

	cout << "测试重载==：" << d1.operator==(d2) << endl;

	cout << "测试重载!=：" << d1.operator!=(d2) << endl;

	cout << "测试重载>：" << d1.operator>(d2) << endl;

	cout << "测试重载<：" << d1.operator<(d2) << endl;

	cout << "测试重载>=：" << d1.operator>=(d2) << endl;

	cout << "测试重载<=：" << d1.operator<=(d2) << endl;

	Date d3 = d2.operator+(10);
	d3.Show();

	d2.operator+=(20);
	d2.Show();

	Date d4 = d2.operator-(-30);
	d4.Show();

	d2.operator-=(30);
	d2.Show();

	Date d5 = d2++;
	d5.Show();

	++d2;
	d2.Show();

	--d2;
	d2.Show();

	Date d6 = d2--;
	d6.Show();

	Date d(2018, 3, 27);
	Date d7(2018, 9, 10);
	d.Show();
	d7.Show();
	int days = d7 - d;
	cout << " 相差天数：" << days << endl;

	system("pause");
	return 0;
}