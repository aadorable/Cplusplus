#pragma once

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
	
private:
	Node* _root;
};
