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
private:
	Node* _root;
};
