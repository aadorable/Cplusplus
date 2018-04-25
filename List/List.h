#pragma once 
#include <stdio.h>
#include <iostream>
#include <assert.h>
using namespace std;

typedef int DataType;

struct ListNode
{
	ListNode* _next;      //存放下一个节点地址
	ListNode* _prev;		 //存放上一个节点地址
	DataType _data;

	ListNode(DataType x)
		:_data(x)
		, _next(NULL)
		, _prev(NULL)
	{}
};

class List
{
	typedef ListNode Node;
public:
	List()
		:_head(new Node(DataType()))
	{
		_head->_next = _head;
		_head->_prev = _head;
	}

	List(const List& l)
		:_head(new Node(DataType()))
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

	List& operator=(List& l)
	{
		if (this != &l)
		{
			swap(_head, l._head);
		}
		return *this;
		
	}

	~List()
	{
		Node* cur = _head->_next;
		while (cur != _head)
		{
			Node* next = cur->_next;
			delete cur;
			cur = next;
		}
		delete _head;
		_head = NULL;
	}

	void Print() const
	{
		Node* cur = _head->_next;
		cout << "head->";
		while (cur != _head)
		{
			cout << cur->_data << "->";
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

	void PushBack(DataType x);
	void PushFront(DataType x);
	void PopBack();
	void PopFront();
	ListNode* Find(DataType x);
	void Insert(Node* pos, DataType x);
	void Erase(Node* pos);
private:
	Node* _head;
};


void List::PushBack(DataType x)
{
	Node* tail = _head->_prev;
	Node* new_node = new Node(x);
	tail->_next = new_node;
	new_node->_prev = tail;

	new_node->_next = _head;
	_head->_prev = new_node;
	//Insert(_head, x);
}

void List::PushFront(DataType x)
{
	Node* cur = _head->_next;
	Node* new_node = new Node(x);
	new_node->_next = cur;
	cur->_prev = new_node;

	new_node->_prev = _head;
	_head->_next = new_node;
	//Insert(_head->_next, x);
}

void List::PopBack()
{
	Node* to_delete = _head->_prev;
	Node* cur = to_delete->_prev;
	cur->_next = _head;
	_head->_prev = cur;	
	delete to_delete;
	//Erase(_head->_prev);
}

void List::PopFront()
{
	Node* to_delete = _head->_next;
	Node* cur = to_delete->_next;
	cur->_prev = _head;
	_head->_next = cur;
	delete to_delete;
	//Erase(_head->_next);
}

ListNode* List::Find(DataType x)
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

void List::Insert(Node* pos, DataType x)
{
	assert(pos);
	Node* prev = pos->_prev;
	Node* new_node = new Node(x);
	new_node->_next = pos;
	pos->_prev = new_node;

	prev->_next = new_node;
	new_node->_prev = prev;
}

void List::Erase(Node* pos)
{
	assert(pos);
	Node* prev = pos->_prev;
	Node* next = pos->_next;

	prev->_next = next;
	next->_prev = prev;
	delete pos;
}

void TestList()
{
	List l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PopBack();
	l.Print();
	ListNode* pos = l.Find(2);
	printf("pos->_data expext 2, actual %d:[%p]\n", pos->_data, pos);
	pos = l.Find(4);
	printf("pos->_data expext NULL, actual [%p]\n", pos);
	pos = l.Find(1);
	printf("pos->_data expext 1, actual %d:[%p]\n", pos->_data, pos);
	l.Insert(pos, 0);
	l.Print();
	l.Erase(pos);
	l.Print();
	
	List l1(l);
	l1.PushFront(8);
	l1.PushFront(7);
	l1.PushFront(6);
	l1.PushFront(5);
	l1.PopFront();
	l1.Print();

	List l2;
	l2 = l;
	l2.Print();
}
