#pragma once

#include <stdlib.h>
#include <iostream>

using namespace std;

enum Color
{
	RED,
	BLACK,
};

template<class T>
struct RBTreeNode
{
	T _data;
	
	RBTreeNode<T>* _left;
	RBTreeNode<T>* _right;
	RBTreeNode<T>* _parent;

	Color _color;   //颜色

	RBTreeNode(const T& x)
		:_data(x)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _color(RED)               //默认结点颜色是红色
	{}
};

template<class T>
struct _RBTreeIterator
{
	typedef RBTreeNode<T> Node;
	typedef _RBTreeIterator<T> Self;
	Node* _node;

	_RBTreeIterator(Node* node)
		:_node(node)
	{}

	T& operator*()
	{
		return _node->_data;
	}

	T* operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		//1.右为空，找祖先里面不为右的
		//2.右不为空，找右树中序第一个
		if (_node->_right)
		{
			_node = _node->_right;
			while (_node->_left)
			{
				_node = _node->_left;
			}
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent)
			{
				if (cur == parent->_right)
				{
					cur = parent;
					parent = cur->_parent;
				}
				else
				{
					break;
				}
			}
			_node = parent;
		}
		return *this;
	}

	Self operator++(int)
	{
		Self tmp(*this);
		++(*this);
		return tmp;
	}

	Self operator--()
	{
		//1.左为空，找祖先里面不为左的
		//2.左不为空，找左树中序最后一个
		if (_node->_left)
		{
			_node = _node->_left;
			while (_node->_right)
			{
				_node = _node->_right;
			}
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent)
			{
				if (cur == parent->_left)
				{
					cur = parent;
					parent = cur->_parent;
				}
				else
				{
					break;
				}
			}
			_node = parent;
		}
		return *this;
	}

	Self operator--(int)
	{
		Self tmp(*this);
		--(*this);
		return tmp;
	}

	bool operator==(const Self& s)
	{
		return _node == s._node;
	}

	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}
};

template<class K, class T, class KeyOfValue>
class RBTree
{
	typedef RBTreeNode<T> Node;
public:
	typedef _RBTreeIterator<T> Iterator;
	RBTree()
		:_root(NULL)
	{}

	Iterator Begin()
	{
		Node* cur = _root;
		while (cur && cur->_left)
		{
			cur = cur->_left;
		}
		return Iterator(cur);
	}
	Iterator End()
	{
		return Iterator(NULL);
	}
	pair<Iterator, bool> Insert(const T& data)
	{
		if (_root == NULL)                 //空树时直接插入
		{
			_root = new Node(data);
			_root->_color = BLACK;
			return make_pair(Iterator(_root), true);
		}
		Node* parent = NULL;
		Node* cur = _root;

		KeyOfValue kov;
		while (cur)                       //寻找插入的位置
		{
			if (kov(cur->_data) < data)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (kov(cur->_data) > data)
			{
				parent = cur;
				cur = cur->_left;
			}
			else{
				return make_pair(Iterator(cur), false);           //要出入节点已经存在，不支持插入
			}
		}
		cur = new Node(data);
		Node* new_node = cur;
		cur->_color = RED;
		if (kov(parent->_data) < data)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		while (parent && parent->_color == RED)              //父节点为红色，需要进行调整
		{
			Node* grand = parent->_parent;                  //祖父结点
			if (parent == grand->_left)
			{
				Node* uncle = grand->_right;                //叔叔节点
				//1.叔叔节点存在且为红
				if (uncle && uncle->_color == RED)
				{
					parent->_color = uncle->_color = BLACK;
					grand->_color = RED;

					cur = grand;
					parent = cur->_parent;                 //继续向上判断
				}
				//2.叔叔节点不存在，存在且为黑
				else
				{
					if (cur == parent->_right)
					{
						RotateL(parent);                  //先对parent进行左旋
						swap(cur, parent);
					}
					RotateR(grand);                      //对祖父结点右旋
					parent->_color = BLACK;
					grand->_color = RED;
				}
			}
			else
			{
				Node* uncle = grand->_left;
				//1.叔叔节点存在且为红
				if (uncle && uncle->_color == RED)
				{
					parent->_color = BLACK;
					uncle->_color = BLACK;
					grand->_color = RED;

					cur = grand;
					parent = cur->_parent;
				}
				//2.叔叔节点不存在，存在且为黑
				else
				{
					if (cur == parent->_left)
					{
						RotateR(parent);
						swap(cur, parent);
					}
					RotateL(grand);
					parent->_color = BLACK;
					grand->_color = RED;
				}
			}
		}
		_root->_color = BLACK;               //强制将根节点变为黑色
		return make_pair(Iterator(new_node), true);
	}
	Iterator Find(const K& key)
	{
		Node* cur = _root;
		KeyOfValue kov;
		while (cur)
		{
			if (kov(cur->_data) < key)
			{
				cur = cur->_right;
			}
			else if (kov(cur->_data) > key)
			{
				cur = cur->_left;
			}
			else
			{
				return Iterator(cur);
			}
		}
		return End();
	}
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}
	bool IsBalance()
	{
		if (_root == NULL)
		{
			return true;
		}
		if (_root->_color == RED)
		{
			return false;
		}
		int count = 0;             //用count统计最左支路的黑节点个数
		Node* cur = _root;
		while (cur)
		{
			if (cur->_color == BLACK)
			{
				++count;
			}
			cur = cur->_left;
		}
		int num = 0;
		return _IsBalance(_root, count, num);
	}
protected:
	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* ppNode = parent->_parent;

		parent->_left = subLR;
		if (subLR)
		{
			subLR->_parent = parent;
		}

		subL->_right = parent;
		parent->_parent = subL;

		if (ppNode == NULL)
		{
			_root = subL;
			subL->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subL;
			}
			else
			{
				ppNode->_right = subL;
			}
			subL->_parent = ppNode;
		}
	}
	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* ppNode = parent->_parent;

		parent->_right = subRL;
		if (subRL)
		{
			subRL->_parent = parent;
		}
		subR->_left = parent;
		parent->_parent = subR;

		if (ppNode == NULL)
		{
			_root = subR;
			subR->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subR;
			}
			else
			{
				ppNode->_right = subR;
			}
			subR->_parent = ppNode;
		}
	}
	void _InOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}
	bool _IsBalance(Node* root, const int& count, int num)
	{
		if (root == NULL)
		{
			return num == count;
		}
		//存在连续红节点，表示不平衡
		if (root->_color == RED && root->_parent->_color == RED)
		{
			cout << "存在连续红节点：" << root->_key << endl;
			return false;
		}
		if (root->_color == BLACK)
		{
			++num;
		}
		return _IsBalance(root->_left, count, num) && _IsBalance(root->_right, count, num);
	}
private:
	Node* _root;
};
