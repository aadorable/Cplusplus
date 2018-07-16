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
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
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

	bool Remove(const K& key)
	{
		Node* parent = NULL;
		Node* cur = _root;
		Node* del = NULL;       //指向要删除的位置
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
				break;
		}
		if (cur == NULL)      //无此节点，删除失败
		{
			return false;
		}
		del = cur;
		//如果要删除的结点有两个孩子，则需要找到右子树的最左结点
		if (cur->_left != NULL && cur->_right != NULL)
		{
			cur = cur->_right;
			while (cur->_left)
			{
				cur = cur->_left;
			}
			del->_key = cur->_key;
			del->_value = cur->_value;
			del = cur;                //交换之后使del指向要删除的结点
		}
		parent = cur->_parent;         //找到要删除结点的父亲
		if (cur->_left == NULL)        //要删除结点的左孩子为空，或者都为空
		{
			if (parent == NULL)       //要删除的是头结点
			{
				_root = cur->_right;
				if (cur->_right)
				{
					cur->_right->_parent = NULL;
				}
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
			cur = del->_right;         //cur更新到要删除节点的右子树
		}
		else                          //要删除节点的右孩子为空，左孩子不为空
		{
			if (parent == NULL)        //要删除的是头结点
			{
				_root = cur->_left;
				if (cur->_left)
				{
					cur->_left->_parent = NULL;
				}
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
				if (cur->_left)
				{
					cur->_left->_parent = parent;
				}
			}
			cur = del->_left;          //cur更新到要删除结点的左子树
		}
		//因为要删除的结点之后的子树的高度不变，不需修改，需要从parent向上判断是否平衡
		while (parent)
		{
			//调整parent的平衡因子
			if (parent->_left == cur)           //删除的是parent的左子树
			{
				parent->_bf++;
			}
			else
			{
				parent->_bf--;
			}
			if (parent->_bf == 1 || parent->_bf == -1)    //原来平衡，删除一个后高度不变，整棵树已经平衡
			{
				break;
			}
			//平衡因子原来不为0，删除一个后变为0，需要继续向上寻找
			if (parent->_bf != 0)
			{
				if (cur == NULL)
				{
					if (parent->_left == NULL)
					{
						cur = parent->_right;
					}
					else
					{
						cur = parent->_left;
					}
				}
				else
				{
					if (parent->_left == cur)          //原来parent比较矮的左子树被删除，让cur指向较高的子树
					{
						cur = parent->_right;
					}
					else
					{
						cur = parent->_left;
					}
				}
				if (cur->_bf == 0)                   //但旋转就可以实现平衡
				{
					if (parent->_bf < 0)             //左子树高，进行右单旋
					{
						RotateR(parent);
						parent->_bf = 1;
						parent->_right->_bf = -1;
					}
					else                            //右子树高，进行左单旋
					{
						RotateL(parent);
						parent->_bf = -1;
						parent->_left->_bf = 1;
					}
					break;
				}
				//如果parent与较高子树同号，进行单旋
				int d = parent->_bf - cur->_bf;
				if (d == 1 || d == -1)
				{
					if (d == 1)                    //右子树高，进行左单旋
					{ 
						RotateL(parent);
					}
					else                          //左子树高，进行右单旋
					{
						RotateR(parent);
					}
				}
				else                              //不同号，进行双旋
				{
					if (d == 3)
					{
						RotateRL(parent);          //parent的平衡因子2，cur的平衡因子为-1，进行右左双旋
					}
					else
					{
						RotateLR(parent);          //parent的平衡因子-2，cur的平衡因子为1，进行左右双旋
					}
				}
			}
			cur = parent;
			parent = parent->_parent;
		}
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

void TestAVLTree()
{
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> t;
	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		t.Insert(a[i], i);
		cout << a[i] << ":" << t.IsBalance() << endl;
	}
	t.InOrder();
	cout << t.IsBalance() << endl;
}
