#pragma once
#include <vector>
//���
//С��
template<class T, class Compare>
class Heap
{
public:
	Heap()
	{}

	Heap(T* a, size_t n)
	{
		_a.reserve(n);
		for (size_t i = 0; i < n; ++i)
		{
			_a.push_back(a[i]);
		}
		//����
		for (int i = (_a.size() - 2) / 2; i >= 0; --i)
		{
			AdjustDown(i);
		}
	}

	void Push(const T&x)
	{
		_a.push_back(x);
		AdjustUp(_a.size() - 1);
	}

	void Pop()
	{
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		AdjustDown(0);
	}

	bool Empty(){
		return _a.empty();
	}

	size_t Size(){
		return _a.size();
	}

	const T& Top()
	{
		return _a[0];
	}

	void AdjustUp(size_t child)
	{
		Compare com;
		size_t parent = (child - 1) >> 1;
		while (child > 0){
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) >> 1;
			}
			else
			{
				break;
			}
		}
	}

	void AdjustDown(size_t root)
	{
		Compare com;
		int parent = root;
		int child = parent * 2 + 1;
		while (child < _a.size())
		{
			//ѡ���Һ�����������һ��
			if (child + 1 < _a.size() && com(_a[child + 1], _a[child]))
			{
				++child;
			}
			//���Ӵ��ڸ����򽻻����Ҽ������µ�
			if (com(_a[child], _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
protected:
	std::vector<T> _a;
};

template<class T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template<class T>
struct Greater
{
	bool operator()(const T& l, const T& r)
	{
		return l > r;
	}
};

void TestHeap()
{
	int a[] = { 10, 11, 13, 12, 16, 18, 15, 14, 17, 19 };
	Heap<int, Less<int>> hp(a, sizeof(a) / sizeof(a[0]));
	hp.Push(12);
	hp.Push(20);
	hp.Push(16);
}