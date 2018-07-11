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
		}
		else
		{
			parent->_left = cur;
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
