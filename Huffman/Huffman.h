//huffmanѹ��
//1.ͳ���ַ����ִ���
//2.����huffman��
//3.����huffman code
//4.ѹ��
//5.��ѹ���������huffman����

#pragma once
#include "Heap.h"

template<class W>
struct HuffmanTreeNode
{
	W _w;      //Ȩֵ
	HuffmanTreeNode<W>* _left;
	HuffmanTreeNode<W>* _right;

	HuffmanTreeNode(const W& w)
		:_left(NULL)
		, _right(NULL)
		, _w(w)
	{}
};

template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;

public:
	HuffmanTree()
		:_root(NULL)
	{}
	//��һ�������ѵ���invalid
	HuffmanTree(W* w, size_t n, const W& invalid)
	{
		struct PNodeCompare
		{
			bool operator()(Node* l, Node* r)
			{
				return l->_w < r->_w;
			}
		};
		//����huffman��
		Heap<Node*, PNodeCompare> minheap;
		for (size_t i = 0; i < n; ++i)
		{
			if (w[i] != invalid)
				minheap.Push(new Node(w[i]));
		}
		while (minheap.Size() > 1)
		{
			Node* left = minheap.Top();
			minheap.Pop();
			Node* right = minheap.Top();
			minheap.Pop();

			Node* parent = new Node(left->_w + right->_w);
			parent->_left = left;
			parent->_right = right;
			minheap.Push(parent);
		}
		_root = minheap.Top();
	}
	~HuffmanTree()
	{
		Destory(_root);
		_root = NULL;
	}
	void Destory(Node* root)
	{
		if (root == NULL)
			return;
		Destory(root->_left);
		Destory(root->_right);
		delete root;
	}

	Node* GetRoot()
	{
		return _root;
	}

private:
	HuffmanTree(const HuffmanTree<W>&);
	HuffmanTree<W>& operator=(const HuffmanTree<W>&);
protected:
	Node* _root;
};

void TestHuffmanTree()
{
	int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	HuffmanTree<int> t(a, sizeof(a)/sizeof(a[0]), 0);
}