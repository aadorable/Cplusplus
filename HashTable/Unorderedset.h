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

void TestUnorderedset()
{
	Unorderedset<int> us;
	us.Insert(2);
	us.Insert(100);
	us.Insert(20);
	us.Insert(24);
	us.Insert(10);
	us.Insert(34);

	for (size_t i = 0; i < 53; ++i)
	{
		us.Insert(i);
	}

	Unorderedset<int>::Iterator it = us.Begin();
	while (it != us.End())
	{
		cout << *it << endl;
		++it;
	}

	cout << endl;
}
