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
	RBTree()
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
	bool Remove(const K& key)
	{
		Node* cur = _root;
		Node* parent = NULL;
		Node* del = NULL;
		while (cur)                 //寻找待删除结点
		{
			if (cur->_key > key)
			{
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				cur = cur->_right;
			}
			else
			{
				break;
			}
		}
		if (cur == NULL)
		{
			return false;                //要删除节点不存在，删除失败
		}
		del = cur;
		//如果要删除的结点有两个孩子，则需找到右子树的最左结点
		if (cur->_left != NULL && cur->_right != NULL)
		{
			cur = cur->_right;
			while (cur->_left)
			{
				cur = cur->_left;
			}
			del->_key = cur->_key;
			del->_value = cur->_value;
			del = cur;                  //交换后使del指向要删除的结点
		}
		parent = cur->_parent;           //找到要删除节点的父亲
		if (cur->_left == NULL)          //要删除节点的左孩子为空，或者都为空
		{ 
			if (parent == NULL)          //要删除的是头结点
			{
				_root = cur->_right;
				if (cur->_right)
				{
					_root->_parent = NULL;
					_root->_color = BLACK;        //记得根节点永远是黑色
				}
				delete del;
				return true;
			}
			else
			{
				if (parent->_left == cur)
				{
					parent->_left = cur->_right;
				}
				else
				{
					parent->_right = cur->_right;
				}
				if (cur->_right)
				{
					cur->_right->_parent = parent;
				}
			}
			cur = del->_right;               //cur更新到要删除节点的右子树
		}
		else                                //要删除节点的右孩子为空
		{
			if (parent == NULL)              //要删除节点为根节点
			{
				_root = cur->_left;
				_root->_parent = NULL;
				_root->_color = BLACK;
				delete del;
				return true;
			}
			else
			{
				if (parent->_left == cur)
				{
					parent->_left = cur->_left;
				}
				else
				{
					parent->_right = cur->_left;
				}
				cur->_left->_parent = parent;
			}
			cur = del->_left;
		}

		//根据颜色判断删除后是否还是合法的红黑树，若不合法则旋转
		if (del->_color == RED)            //要删除结点是红色，则删除后任然平衡，无需旋转
		{
			delete del;
			return true;
		}
		if (del->_color == BLACK && cur && cur->_color == RED)     //如果要删除节点是黑色，且它的孩子是红色			
		{														//则将孩子变为黑色即可
			cur->_color = BLACK;
			delete del;
			return true;
		}
		//要删除节点是黑色，且它的孩子为NULL或为黑色
		while (parent)
		{
			if (parent->_left == cur)           //要删除节点是父节点的左孩子
			{
				Node* subR = parent->_right;
				if (subR->_color == RED)
				{
					RotateL(parent);            //对parent左旋，染色
					subR->_color = BLACK;
					parent->_color = RED;
				}
				else                           //subR是黑色的
				{
					Node* subRL = subR->_left;
					Node* subRR = subR->_right;
					if (parent->_color == BLACK && (subRL == NULL && subRR == NULL) ||
						(subRL && subRL->_color == BLACK && subRR && subRR->_color == BLACK))
					{
						subR->_color = RED;     //使subR这条路径上减少一个黑色结点，再向上判断
						cur = parent;
						parent = cur->_parent;
					}
					else
					{
						if (parent->_color == RED)
						{
							if ((subRL == NULL && subRR == NULL) ||
								(subRL && subRL->_color == BLACK && subRR && subRR->_color == BLACK))
							{
								parent->_color = BLACK;     //将父节点变为黑色
								subR->_color = RED;
								break;
							}
						}
						if (subRL->_color == RED)           //subRL为红色，先对subR进行右旋转换为左单旋情况
						{
							RotateR(subR);
							subR = subRL;
						}
						RotateL(parent);
						//将旋转后新的父节点变为与原来父节点一样的颜色
						if (parent->_color == RED)         
						{
							subR->_color = RED;
						}
						else
						{
							subR->_color = BLACK;
						}
						//将原来的父节点染黑
						parent->_color = BLACK;
						subR->_right->_color = BLACK;       //由于subR的右子树少一个黑结点
						break;
					}
				}
			}
			else                                          //要删除节点是父节点的右孩子
			{
				Node* subL = parent->_left;
				if (subL->_color == RED)
				{
					RotateR(parent);                       //对parent右旋，染色
					parent->_color = RED;
					subL->_color = BLACK;
				}
				else                                      //parent的左孩子是黑的
				{
					Node* subLR = subL->_right;
					Node* subLL = subL->_left;
					//如果父节点和subL都是黑的，subL的孩子要么全空，要么全黑
					if (parent->_color == BLACK && (subLL == NULL && subLR == NULL) ||
						(subLL && subLL->_color == BLACK && subLR && subLR->_color == BLACK))
					{
						subL->_color = RED;               //使subL这条路径上减少一个黑色结点，再向上判断
						cur = parent;
						parent = cur->_parent;
					}
					else
					{
						if (parent->_color == RED)
						{
							if ((subLL == NULL && subLR == NULL) ||
								(subLL && subLL->_color == BLACK && subLR && subLR->_color == BLACK))
							{
								parent->_color = BLACK;            //cur路径增加一个黑节点
								subL->_color = RED;                //subL路径黑节点个数不变
								break;
							}
						}
						if (subLR->_color == RED)                  //subLR为红色，先对subL进行左旋转换为右单旋情况
						{
							RotateL(subL);
							subL = subLR;
						}
						RotateR(parent);
						//将旋转后新的父节点变为与原来父节点一样的颜色
						if (parent->_color == RED)
						{
							subL->_color = RED;
						}
						else
						{
							subL->_color = BLACK;
						}
						//将原来的父节点染黑
						parent->_color = BLACK;
						subL->_left->_color = BLACK;
						break;
					}
				}
			}
		}

		_root->_color = BLACK;
		delete del;
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
