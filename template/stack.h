#pragma once

//适配器模式
//模板的模板参数

template<class T, class Container>
class Stack
{
public:
	void Push(const T& x)
	{
		_con.PushBack(x);
	}

	void Pop()
	{
		_con.PopBack();
	}

	const T& Top() const
	{
		return _con.Back();
	}

	size_t Size()
	{
		_con.Size();
	}

	bool Empty()
	{
		return _con.Empty();
	}

protected:
	Container _con;
};

void TestStack()
{
	Stack<int, Vector<int>> s;
	s.
}