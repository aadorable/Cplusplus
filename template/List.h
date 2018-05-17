#pragma once

template<class T>
struct ListNode
{
	ListNode<T>* _next;
	ListNode<T>* _prev;
	T _data;

	ListNode(const T& x)
		:_data(x)
		, _next(NULL)
		, _prev(NULL)
	{}
};

template<class T>
class List
{
	typedef ListNode<T> Node;
public:
	List()
	{
		_head = new Node(T());
		_head->_next = _head;
		_head->_prev = _head;
	}

	List(const List<T>& l)
		:_head(new Node(T()))
	{
		_head->_next = _head;
		_head->_prev = _head;
		Node* cur = l._head->_next;
		while (cur != l._head)
		{
			PushBack(cur->_data);
			cur = cur->_next;
		}
	}

	List<T>& operator=(const Vector<T>& l)
	{
		if (this != &l)
		{
			swap(_head, l._head);
		}
		return *this;
	}

	~List()
	{
		Clear();
		delete _head;
		_head = NULL;
	}

	void Clear()
	{
		Node* cur = _head->_next;
		while (cur != _head)
		{
			Node* next = cur->_next;
			delete cur;
			cur = next;
		}
		_head->_next = _head;
		_head->_prev = _head;
	}

	void PushBack(const T& x);
	void PopBack();
	void PushFront(const T& x);
	void PopFront();

	void Insert(Node* pos, const T& x);
	void Erase(Node* pos);
	Node* Find(const T& x)
	{
		Node* cur = _head->_next;
		while (cur != _head)
		{
			if (cur->_data == x)
			{
				return cur;
			}
			cur = cur->_next;
		}
		return NULL;
	}
	void Print() const;

protected:
	Node* _head;
};

template<class T>
void List<T>::Print() const
{
	Node* cur = _head->_next;
	cout << "head";
	while (cur != _head)
	{
		cout << "->" << cur->_data;
		cur = cur->_next;
	}
	cout << endl;
	Node* tail = _head->_prev;
	while (tail != _head)
	{
		cout << tail->_data << "->";
		tail = tail->_prev;
	}
	cout << "head" << endl;
}

template<class T>
void List<T>::PushBack(const T& x)
{
	Insert(_head, x);
}

template<class T>
void List<T>::PopBack()
{
	Erase(_head->_prev);
}

template<class T>
void List<T>::PushFront(const T& x)
{
	Insert(_head->_next, x);
}

template<class T>
void List<T>::PopFront()
{
	Erase(_head->_next);
}

template<class T>
void List<T>::Insert(Node* pos, const T& x)
{
	assert(pos);
	Node* prev = pos->_prev;
	Node* new_node = new Node(x);
	prev->_next = new_node;
	new_node->_prev = prev;
	new_node->_next = pos;
	pos->_prev = new_node;
}

template<class T>
void List<T>::Erase(Node* pos)
{
	assert(pos && pos != _head);
	Node* prev = pos->_prev;
	Node* next = pos->_next;
	prev->_next = next;
	next->_prev = prev;
	delete pos;
}

void TestList()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.Print();
	ListNode<int>* cur = l.Find(4);
	l.Insert(cur, 5);
	l.Print();

	List<int> l1(l);
	l1.Print();
}