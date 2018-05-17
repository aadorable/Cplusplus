#pragma once

template<class T, class Container>
class Queue
{
public:
	void Push(const T& x)
	{
		_con.PushBack(x);
	}

	void Pop()
	{
		_con.PopFront();
	}

	const T& Front() const
	{
		return _con.Front();
	}

	bool Empty() const
	{
		return _con.Empty();
	}
protected:
	Container _con;
};

void TestQueue()
{
	Queue<int, List<int>> q;
	q.Push(1);
	q.Push(2);
	q.Push(3);
	q.Push(4);

	while (!q.Empty())
	{
		cout << q.Front() << " ";
		q.Pop();
	}
	cout << endl;
}