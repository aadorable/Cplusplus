#pragma once

#include <stdlib.h>
#include <iostream>

using namespace std;

enum Color
{
	RED,
	BLACK,
};

template<class K, class V>
struct RBTreeNode
{
	K _key;
	V _value;
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;

	Color _color;   //颜色

	RBTreeNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _color(RED)
	{}
};

template<class K, class V>
struct RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBtree()
		:_root(NULL)
	{}
	RBTree(const RBTree<K, V>& tree)
	{
		_Copy(tree._root, _root);
	}
	RBTree<K, V>& operator=(const RBTree<K, V>& tree)
	{
		if (this != &tree)
		{
			RBTree<K, V> tmp(tree);
			swap(_root, tmp._root);
		}
		return *this;
	}
	~RBTree()
	{
		_Destory(_root);
	}
	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value);
			_root->_color = BLACK;
			return true;
		}
		Node* parent = NULL;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else{
				return false;
			}
		}
		cur = new Node(key, value);
		cur->_color = RED;
		if (parent->_key < key)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->left = cur;
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
		return true;
	}
	bool Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->_key < key)
			{
				cur = cur->_right;
			}
			else if (cur->_key > key)
			{
				cur = cur->_left;
			}
			else
			{
				return true;
			}
		}
		return false;
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
	void _Copy(Node* root, Node* new_root)
	{
		if (root == NULL)
		{
			return;
		}
		Node* cur = new Node(root->_key, root->_value);
		cur->_color = root->_color;
		
		new_root = cur;
		cur->_parent = new_root;
		_Copy(root->_left, cur->_left);
		_Copy(root->_right, cur->_right);
	}
	void _Destory(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_Destory(root->_left);
		_Destory(root->_right);
		delete root;
	}
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

void TestRBTree()
{
	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	RBTree<int, int> t;
	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		t.Insert(a[i], i);
		cout << a[i] << ":" << t.IsBalance() << endl;
	}
	t.InOrder();
	cout << t.IsBalance() << endl;
	cout << t.Find(16) << endl;
	cout << t.Find(10) << endl;
	t.Remove(11);
	t.InOrder();
	cout << t.IsBalance() << endl;
}
