#pragma once 

#include "HashTable.h"

template<class K, class V, class HashFunc = Hash<K>>
class Unorderedmap
{
	struct MapKeyOfValue
	{
		const K& operator()(const pair<K, V>& kv)
		{
			return kv.first;
		}
	};

	typedef typename HashTable<K, pair<K, V>, MapKeyOfValue, HashFunc>::Iterator Iterator;

	pair<Iterator, bool> Insert(const pair<K, V>& kv)
	{
		return _ht.Insert(kv);
	}
protected:
	HashTable<K, pair<K, V>, MapKeyOfValue, HashFunc> _ht;
};
