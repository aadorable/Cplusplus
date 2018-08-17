#pragma once

#include "HashTable.h"

template<class K, class HashFunc = Hash<K>>
class Unorderedset
{
	struct SetKeyOfValue
	{
		const K& operator()(const K& key)
		{
			return key;
		}
	};
public:
	typedef typename HashTable<K, K, SetKeyOfValue, HashFunc>::Iterator Iterator;

	pair<Iterator, bool> Insert(const K& key)
	{
		return _ht.Insert(key);
	}

	Iterator Begin()
	{
		return _ht.Begin();
	}

	Iterator End()
	{
		return _ht.End();
	}
protected:
	HashTable<K, K, SetKeyOfValue, HashFunc> _ht;
};
