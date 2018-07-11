#pragma once

#include <iostream>

using namespace std;

template<class K, class V>
struct AVLTreeNode
{
	K _key;
	V _value;
	int _bf;       //平衡因子，右子树高度减去左子树高度
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;

	AVLTreeNode(K key, V value)
		:_key(key)
		, _value(value)
		, _bf(0)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
	{}
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}
	AVLTree(const AVLTree<K, V>& tree)
		:_root(NULL)
	{
		_Copy(_root, tree._root);
	}
	AVLTree<K, V>& operator=(const AVLTree<K, V>& tree)
	{
		if (this != &tree)
		{
			AVLTree<K, V> tmp(tree);
			swap(_root, tmp._root)
		}
		return *this;
	}
	~AVLTree()
	{
		_Destroy(_root);
	}

	bool Insert(const K& key, const V& value)
	{
		if (root == NULL)
		{
			_root = new Node(key, value);    //空树时直接插入
			return true;
		}
		Node* parent = NULL;
		Node* cur = _root;
		while (cur)                 //寻找插入的位置
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
			else
			{
				return false;       //如果已经存在该元素，则插入失败
			}
		}
		cur = new Node(key, value);
		if (parent->_key < key)
		{
			parent->_right = cur;
		}
		else
		{
			parent->_left = cur;
		}

		//调节平衡因子
		//1.更新平衡因子
		while (parent)
		{
			if (cur == parent->_right)     //插入节点为parent的右节点，parent平衡因子++
			{
				parent->_bf++;
			}
			else                         //插入节点为parent的左节点，parent平衡因子--
			{
				parent->_bf--;
			}

			if (parent->_bf == 0)         //父亲bf==0，表示树的高度不变，此时就是AVL树
			{
				break;
			}
			else if (parent->_bf == -1 || parent->_bf == 1)        //子树高度变了，继续往上更新
			{
				cur = parent;
				parent = parent->_parent;
			}
			else                        //父亲bf==2或-2，不再满足AVL树，需要旋转恢复平衡
			{
				//旋转
				if (parent->_bf == 2)      //parent的右子树高
				{
					if (cur->_bf == 1)     //parent的子树也是右子树高，进行左单旋
					{
						RotateL(parent);
					}
					else                  //parent的子树的左子树高，进行右左双旋
					{
						RotateRL(parent);  
					}
				}
				else                      //parent的左子树高
				{
					if (cur->_bf == -1)    //parent的子树也是左子树高，进行右单旋
					{
						RotateR(parent);
					}
					else                  //parent的子树的右子树高，进行左右双旋
					{
						RotateLR(parent);
					}
				}
				break;                    //旋转完成后树恢复平衡
			}
		}
		return true;
	} 

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

protected:
	void _Copy(Node* root, Node* new_root)
	{
		if (root == NULL)
		{
			return;
		}
		Node* node = new Node(root->_key, root->_value);
		node->_bf = root->_bf;
		new_root = node;
		node->_parent = new_root;
		_Copy(root->_left, new_root->_left);
		_Copy(root->_right, new_root->_right);
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
private:
	Node* _root;
};
