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
private:
	Node* _root;
};
