#pragma once

#include <iostream>

using namespace std;

template<class K, class V>
struct AVLTreeNode
{
	K _key;
	V _value;
	int _bf;       //ƽ�����ӣ��������߶ȼ�ȥ�������߶�
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
			_root = new Node(key, value);    //����ʱֱ�Ӳ���
			return true;
		}
		Node* parent = NULL;
		Node* cur = _root;
		while (cur)                 //Ѱ�Ҳ����λ��
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
				return false;       //����Ѿ����ڸ�Ԫ�أ������ʧ��
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

		//����ƽ������
		//1.����ƽ������
		while (parent)
		{
			if (cur == parent->_right)     //����ڵ�Ϊparent���ҽڵ㣬parentƽ������++
			{
				parent->_bf++;
			}
			else                         //����ڵ�Ϊparent����ڵ㣬parentƽ������--
			{
				parent->_bf--;
			}

			if (parent->_bf == 0)         //����bf==0����ʾ���ĸ߶Ȳ��䣬��ʱ����AVL��
			{
				break;
			}
			else if (parent->_bf == -1 || parent->_bf == 1)        //�����߶ȱ��ˣ��������ϸ���
			{
				cur = parent;
				parent = parent->_parent;
			}
			else                        //����bf==2��-2����������AVL������Ҫ��ת�ָ�ƽ��
			{
				//��ת
				if (parent->_bf == 2)      //parent����������
				{
					if (cur->_bf == 1)     //parent������Ҳ���������ߣ���������
					{
						RotateL(parent);
					}
					else                  //parent���������������ߣ���������˫��
					{
						RotateRL(parent);  
					}
				}
				else                      //parent����������
				{
					if (cur->_bf == -1)    //parent������Ҳ���������ߣ������ҵ���
					{
						RotateR(parent);
					}
					else                  //parent���������������ߣ���������˫��
					{
						RotateLR(parent);
					}
				}
				break;                    //��ת��ɺ����ָ�ƽ��
			}
		}
		return true;
	} 

	bool Remove(const K& key)
	{
		Node* parent = NULL;
		Node* cur = _root;
		Node* del = NULL;       //ָ��Ҫɾ����λ��
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
		if (cur == NULL)      //�޴˽ڵ㣬ɾ��ʧ��
		{
			return false;
		}
		del = cur;
		//���Ҫɾ���Ľ�����������ӣ�����Ҫ�ҵ���������������
		if (cur->_left != NULL && cur->_right != NULL)
		{
			cur = cur->_right;
			while (cur->_left)
			{
				cur = cur->_left;
			}
			del->_key = cur->_key;
			del->_value = cur->_value;
			del = cur;                //����֮��ʹdelָ��Ҫɾ���Ľ��
		}
		parent = cur->_parent;         //�ҵ�Ҫɾ�����ĸ���
		if (cur->_left == NULL)        //Ҫɾ����������Ϊ�գ����߶�Ϊ��
		{
			if (parent == NULL)       //Ҫɾ������ͷ���
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
			cur = del->_right;         //cur���µ�Ҫɾ���ڵ��������
		}
		else                          //Ҫɾ���ڵ���Һ���Ϊ�գ����Ӳ�Ϊ��
		{
			if (parent == NULL)        //Ҫɾ������ͷ���
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
			cur = del->_left;          //cur���µ�Ҫɾ������������
		}
		//��ΪҪɾ���Ľ��֮��������ĸ߶Ȳ��䣬�����޸ģ���Ҫ��parent�����ж��Ƿ�ƽ��
		while (parent)
		{
			//����parent��ƽ������
			if (parent->_left == cur)           //ɾ������parent��������
			{
				parent->_bf++;
			}
			else
			{
				parent->_bf--;
			}
			if (parent->_bf == 1 || parent->_bf == -1)    //ԭ��ƽ�⣬ɾ��һ����߶Ȳ��䣬�������Ѿ�ƽ��
			{
				break;
			}
			//ƽ������ԭ����Ϊ0��ɾ��һ�����Ϊ0����Ҫ��������Ѱ��
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
					if (parent->_left == cur)          //ԭ��parent�Ƚϰ�����������ɾ������curָ��ϸߵ�����
					{
						cur = parent->_right;
					}
					else
					{
						cur = parent->_left;
					}
				}
				if (cur->_bf == 0)                   //����ת�Ϳ���ʵ��ƽ��
				{
					if (parent->_bf < 0)             //�������ߣ������ҵ���
					{
						RotateR(parent);
						parent->_bf = 1;
						parent->_right->_bf = -1;
					}
					else                            //�������ߣ���������
					{
						RotateL(parent);
						parent->_bf = -1;
						parent->_left->_bf = 1;
					}
					break;
				}
				//���parent��ϸ�����ͬ�ţ����е���
				int d = parent->_bf - cur->_bf;
				if (d == 1 || d == -1)
				{
					if (d == 1)                    //�������ߣ���������
					{ 
						RotateL(parent);
					}
					else                          //�������ߣ������ҵ���
					{
						RotateR(parent);
					}
				}
				else                              //��ͬ�ţ�����˫��
				{
					if (d == 3)
					{
						RotateRL(parent);          //parent��ƽ������2��cur��ƽ������Ϊ-1����������˫��
					}
					else
					{
						RotateLR(parent);          //parent��ƽ������-2��cur��ƽ������Ϊ1����������˫��
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

		parent->_left = subLR;              //�ı�ָ��
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
		parent->_bf = subL->_bf = 0;         //����ƽ������
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
				cout << "ƽ�������쳣" << root->_key << endl;
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
