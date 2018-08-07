#pragma once

#include <vector>

template<class V>
struct HashNode
{
	V _data;      // set->key   map->pair<k, v>
	HashNode<V>* _next;

	HashNode(const V& v)
		:_data(v)
	{}
};
