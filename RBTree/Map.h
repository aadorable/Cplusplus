#pragma once

#include <string>
#include "RBTree.h"

template<class K, class V>
class Map
{
	struct MapKeyOfValue
	{
		const K& operator()(const pair<K, V>& kv)
		{
			return kv.first;
		}
	};

public:
	typedef typename RBTree<K, pair<K, V>, MapKeyOfValue>::Iterator Iterator;

	pair<Iterator, bool> Insert(const pair<K, V>& kv)
	{
		return _t.Insert(kv);
	}

	V& operator[](const K& key)
	{
		pair<Iterator, bool> ret = Insert(make_pair(key, V()));
		return ret.first->second;
	}

	Iterator Begin()
	{
		return _t.Begin(); 
	}

	Iterator End()
	{
		return _t.End();
	}

	Iterator Find(const K& key)
	{
		return _t.Find(key);
	}

protected:
	RBTree<K, pair<K, V>, MapKeyOfValue> _t;
};

void TestMap()
{
	//Map<string, string> dict;
	//dict.Insert(make_pair("insert", "插入"));
	//dict.Insert(make_pair("value", "值"));
	//dict.Insert(make_pair("key", "关键字"));
	//dict.Insert(make_pair("right", "右边"));
	//dict.Insert(make_pair("left", "左边"));

	//dict["left"] = "修改";
	//dict["key"];

	//Map<string, string>::Iterator it = dict.Begin();
	//while (it != dict.End())
	//{
	//	//cout << (*it).first << ": " << (*it).second << endl;
	//	cout << it->first << ": " << it->second << endl;
	//	++it;
	//}

	string strs[] = { "insert", "first", "end", "first", "insert", "insert" };
	Map<string, int> countMap;
	for (size_t i = 0; i < sizeof(strs) / sizeof(strs[0]); ++i)
	{
		//方法一
		/*Map<string, int>::Iterator ret = countMap.Find(strs[i]);
		if (ret != countMap.End())
		{
			ret->second++;
		}
		else
		{
			countMap.Insert(make_pair(strs[i], 1));
		}*/

		//方法二
		/*pair<Map<string, int>::Iterator, bool> ret = countMap.Insert(make_pair(strs[i], 1));
		if (ret.second == false)
		{
			ret.first->second++;
		}*/

		//方法三
		countMap[strs[i]]++;
	}

	Map<string, int>::Iterator it = countMap.Begin();
	while (it != countMap.End())
	{
		cout << it->first << ": " << it->second << endl;
		++it;
	}
}
