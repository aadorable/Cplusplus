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
};
