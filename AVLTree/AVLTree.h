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
	
	bool IsBalance()
	{
		int height = 0;
		return _IsBalance(_root, height);
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

		if (parent == _root)
		{
			_root = subR;
			_root->_parent = NULL;
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
		parent->_bf = subR->_bf = 0;
	}
	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* ppNode = parent->_parent;

		parent->_left = subLR;              //改变指向
		if (subLR)
		{
			subLR->_parent = parent;
		}
		subL->_right = parent;
		parent->_parent = subL;

		if (ppNode == NULL)
		{
			_root = subL;
			_root->_parent = NULL;
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
		parent->_bf = subL->_bf = 0;         //更新平衡因子
	}
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(parent->_right);
		RotateL(parent);

		if (bf == 0)
		{
			parent->_bf = subR->_bf = subRL->_bf = 0;
		}
		else if (bf == 1)
		{
			subR->_bf = 0;
			subRL->_bf = 0;
			parent->_bf = -1;
		}
		else if (bf == -1)
		{
			subR->_bf = 1;
			subRL->_bf = 0;
			parent->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);

		if (bf == 0)
		{
			parent->_bf = subL->_bf = subLR->_bf = 0;
		}
		else if (bf == 1)
		{
			subL->_bf = -1;
			subLR->_bf = 0;
			parent->_bf = 0;
		}
		else if (bf == -1)
		{
			subL->_bf = 0;
			subLR->_bf = 0;
			parent->_bf = 1;
		}
		else
		{
			assert(false);
		}
	}
	bool _IsBalance(Node* root, int& height)
	{
		if (root == NULL)
		{
			height = 0;
			return true;
		}
		int leftHeight = 0;
		int rightHeight = 0;
		if (_IsBalance(root->_left, leftHeight) && _IsBalance(root->_right, rightHeight)
		{
			if ((rightHeight - leftHeight) != root->_bf)
			{
				cout << "平衡因子异常" << root->_key << endl;
				return false;
			}
			height = leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
			return abs(leftHeight - rightHeight) < 2;
		}
		else
		{
			return false;
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
private:
	Node* _root;
};
