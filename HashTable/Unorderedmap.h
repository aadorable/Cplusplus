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

public:
	typedef typename HashTable<K, pair<K, V>, MapKeyOfValue, HashFunc>::Iterator Iterator;

	pair<Iterator, bool> Insert(const pair<K, V>& kv)
	{
		return _ht.Insert(kv);
	}

	Iterator Begin()
	{
		return _ht.Begin();
	}

	Iterator End()
	{
		return _ht.End();
	}

	V& operator[](const K& key)
	{
		pair<Iterator, bool> ret = _ht.Insert(make_pair(key, V()));
		return ret.first->second;
	}
protected:
	HashTable<K, pair<K, V>, MapKeyOfValue, HashFunc> _ht;
};

struct HashFuncstring
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

void TestUnorderedmap()
{
	Unorderedmap<string, string> dict;
	dict.Insert(make_pair("string", "字符串"));
	dict.Insert(make_pair("map", "映射"));
	dict.Insert(make_pair("insert", "插入"));
	dict["left"] = "左边";
	dict["left"] = "剩余";

	Unorderedmap<string, string>::Iterator it = dict.Begin();
	while (it != dict.End())
	{
		cout << it->first << ":" << it->second << endl;
		++it;
	}
	cout << endl;
}
