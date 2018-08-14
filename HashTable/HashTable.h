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
class HashTable
{
	typedef HashNode<V> Node;

	template<class K, class V, class KeyOfValue, class _HashFunc>
	friend struct _HashTableIterator;
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

	void CheckCapacity()
	{
		if (_size == _tables.size())       // load factor == 1
		{
			size_t new_size = GetNextPrime(_tables.size());

			HashTable<K, V, KeyOfValue> new_table;
			new_table._tables.resize(new_size, NULL);
			for (size_t i = 0; i < _tables.size(); ++i)
			{
				Node* cur = _tables[i];
				while (cur)
				{
					new_table.Insert(cur->_data);
					cur = cur->_next;
				}
			}
			_tables.swap(new_table._tables);
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

