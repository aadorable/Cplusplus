#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

template<class V>
struct HashNode
{
	V _data;      // set->key   map->pair<k, v>
	HashNode<V>* _next;

	HashNode(const V& v)
		:_data(v)
	{}
};

template<class K>
struct Hash
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

template<>
struct Hash<string>
{
	size_t BKDR(const char* str)
	{
		size_t hash = 0;
		while (*str)
		{
			hash = hash * 131 + *str;
			++str;
		}

		return hash;
	}

	size_t operator()(const string& s)
	{
		return BKDR(s.c_str());
	}
};

//前置声明
template<class K, class V, class KeyOfValue, class _HashFunc>
class HashTable;

template<class K, class V, class KeyOfValue, class _HashFunc>
class _HashTableIterator
{
	typedef HashNode<V> Node;
	Node* _node;
	HashTable<K, V, KeyOfValue, _HashFunc>* _ht;
	typedef _HashTableIterator<K, V, KeyOfValue, _HashFunc> Self;

public:
	_HashTableIterator(Node* node, HashTable<K, V, KeyOfValue, _HashFunc>* ht)
		:_node(node)
		, _ht(ht)
	{}

	V& operator*()
	{
		return _node->_data;
	}

	V* operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		if (_node->_next)
		{
			_node = _node->_next;
		}
		else
		{
			KeyOfValue kov;
			size_t index = _ht->HashFunc(kov(_node->_data), _ht->_tables.size()) + 1;
			while (index < _ht->_tables.size())
			{
				if (_ht->_tables[index])
				{
					_node = _ht->_tables[index];
					break;
				}
				else
				{
					++index;
				}
			}
			if (index == _ht->_tables.size())
			{
				_node = NULL;
			}
		}
		return *this;
	}

	bool operator!=(const Self& s) const
	{
		return _node != s._node;
	}
};

template<class K, class V, class KeyOfValue, class _HashFunc>
class HashTable
{
	typedef HashNode<V> Node;

	template<class K, class V, class KeyOfValue, class _HashFunc>
	friend class _HashTableIterator;

public:
	typedef _HashTableIterator<K, V, KeyOfValue, _HashFunc> Iterator;

	HashTable()
		:_size(0)
	{}

	Iterator Begin()
	{
		size_t index = 0;
		while (index < _tables.size())
		{
			if (_tables[index])
			{
				return Iterator(_tables[index], this);
			}
			else
			{
				++index;
			}
		}
		return End();
	}
	Iterator End()
	{
		return Iterator(NULL, this);
	}

	size_t GetNextPrime(size_t cur)
	{
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList[_PrimeSize] =
		{
			53ul,	         97ul,         193ul,       389ul,       769ul,
			1543ul,        3079ul,       6151ul,      12289ul,     24593ul,
			49157ul,       98317ul,      196613ul,    393241ul,    786433ul,
			1572869ul,     3145739ul,    6291469ul,   12582917ul,  25165843ul,
			50331653ul,	  100663319ul,  201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};
		for (size_t i = 0; i < _PrimeSize; ++i)
		{
			if (_PrimeList[i] > cur)
			{
				return _PrimeList[i];
			}
		}
		return _PrimeList[_PrimeSize - 1];
	}

		//void CheckCapacity()
	//{
	//	if (_size == _tables.size())       // load factor == 1
	//	{
	//		size_t new_size = GetNextPrime(_tables.size());

	//		HashTable<K, V, KeyOfValue> new_table;
	//		new_table._tables.resize(new_size, NULL);
	//		for (size_t i = 0; i < _tables.size(); ++i)
	//		{
	//			Node* cur = _tables[i];
	//			while (cur)
	//			{
	//				new_table.Insert(cur->_data);
	//				cur = cur->_next;
	//			}
	//		}
	//		_tables.swap(new_table._tables);
	//	}
	//}

	void CheckCapacity()
	{
		if (_size == _tables.size())     // load factor == 1
		{
			size_t new_size = GetNextPrime(_tables.size());

			vector<Node*> new_table;
			new_table.resize(new_size, NULL);
			KeyOfValue kov;
			for (size_t i = 0; i < _tables.size(); ++i)
			{
				Node* cur = _tables[i];
				while (cur)
				{
					Node* next = cur->_next;
					// cur 取下来，插入到新表中（头插）
					size_t index = HashFunc(kov(cur->_data), _tables.size());
					cur->_next = new_table[index];
					new_table[index] = cur;

					cur = next;
				}
				_tables[i] = NULL;
			}
			_tables.swap(new_table);
		}
	}

	size_t HashFunc(const K& key, size_t size)
	{
		_HashFunc hf;
		return hf(key) % size;
	}
	pair<Iterator, bool> insert(const V& data)
	{
		CheckCapacity();

		KeyOfValue kov;
		size_t index = HashFunc(kov(data), _tables.size());

		Node* cur = _tables[index];
		while (cur)
		{
			if (kov(data) == kov(cur->_data))
			{
				return make_pair(Iterator(cur, this), false);
			}
			cur = cur->_next;
		}
		Node* new_node = new Node(data);
		new_node->_next = _tables[index];
		_tables[index] = new_node;
		++_size;

		return make_pair(Iterator(new_node, this), true);
	}
private:
	vector<Node*> _tables;
	size_t _size;
};

