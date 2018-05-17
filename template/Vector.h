#pragma once

//类名 Vector
//类型 Vector<T>
template<class T>
class Vector
{
public:
	Vector()      //构造函数函数名与类名相同
		:_start(NULL)
		, _finish(NULL)
		, _endofstorage(NULL)
	{}

	Vector<T>(Vector<T>& v)
	{
		if (v.Size() > 0)
		{
			_start = new T[v.Size()];      //只开辟原有数据所占空间大小，节省空间
			memcpy(_start, v._start, sizeof(T)*v.Size());
			if (_start)
			{
				_finish = _start + v.Size();
				_endofstorage = _start + v.Size();
			}
			else
			{
				_start = _finish = _endofstorage = NULL;
			}
		}
	}

	Vector<T>& operator=(Vector<T>& v)
	{
		if (this != &v)
		{
			////传统写法
			//size_t size = v.Size();
			//T* tmp = new T[v.Size()];
			//memcpy(tmp, _start, sizeof(T)*size);
			//delete[] _start;
			//_start = tmp;
			//_finish = _start + size;
			//_endofstorage = _start + size;

			//现代写法
			swap(_start, v._start);
			swap(_finish, v._finish);
			swap(_endofstorage, v._endofstorage);
		}
		return *this;
	}

	~Vector()
	{
		delete[] _start;
		_start = _finish = _endofstorage = NULL;
	}

	void PushBack(const T& x);
	void PopBack();
	void Insert(size_t pos, const T& x);
	void Erase(size_t pos);
	size_t Size();
	size_t Capacity();
	bool Empty();
	size_t Find(const T& x);
	void Print()
	{
		T* cur = _start;
		while (cur != _finish)
		{
			cout << *cur << " ";
			++cur;
		}
		cout << endl;
	}

	const T& Back() const
	{
		return *(_finish - 1);
	}

	T& operator[] (size_t index)
	{
		return _start[index];
	}

protected:
	void Expand(size_t n);

protected:
	T* _start;
	T* _finish;
	T* _endofstorage;
};

//template<class T>
//Vector<T>::Vector()
//	:_start(NULL)
//	, _finish(NULL)
//	, _endofstorage(NULL)
//{}

template<class T>
void Vector<T>::Expand(size_t n)
{
	if (Empty())
	{
		_start = new T[3];
		_finish = _start;
		_endofstorage = _start + 3;
	}
	else if (n > Capacity())
	{
		size_t size = Size();
		T* tmp = new T[n];
		//memcpy(tmp, _start, sizeof(T)*size);
		for (size_t i = 0; i < size; ++i)
		{
			tmp[i] = _start[i];
		}

		delete[] _start;
		_start = tmp;
		_finish = _start + size;
		_endofstorage = _start + n;
	}
}

template<class T>
size_t Vector<T>::Size()
{
	return _finish - _start;
}

template<class T>
size_t Vector<T>::Capacity()
{
	return _endofstorage - _start;
}

template<class T>
bool Vector<T>::Empty()
{
	return _start == _finish;
}

template<class T>
void Vector<T>::PushBack(const T& x)
{
	Insert(Size(), x);
}

template<class T>
void Vector<T>::PopBack()
{
	Erase(Size() - 1);
}

template<class T>
void Vector<T>::Insert(size_t pos, const T& x)
{
	assert(pos <= Size());
	if (Size() >= Capacity())
	{
		Expand(Capacity() * 2);
	}
	T* end = _finish - 1;
	while (end >= _start + pos)
	{
		*(end + 1) = *end;
		--end;
	}
	_start[pos] = x;
	++_finish;
}

template<class T>
void  Vector<T>::Erase(size_t pos)
{
	assert(pos < Size());
	T* cur = _start + pos + 1;
	while (cur != _finish)
	{
		*(cur - 1) = *cur;
		++cur;
	}
	--_finish;
}

template<class T>
size_t Vector<T>::Find(const T& x)
{
	T* cur = _start;
	while (cur != _finish)
	{
		if (*cur == x)
		{
			return cur - _start;
		}
		++cur;
	}
	return (size_t)-1;
}


void TestVector()
{
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);
	v.Print();

	Vector<int> v1(v);
	v1.Insert(2, 5);
	v1.Print();

	Vector<int> v2;
	v2 = v1;
	v2.Insert(3, 6);
	v2.Print();
}