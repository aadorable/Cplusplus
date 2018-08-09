#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <queue>

using namespace std;

template<class V, class W, bool Direction = false>
class GraphMartix
{
public:
	GraphMartix(V* v, size_t n)
	{
		_vertexs.reserve(n);
		for (size_t i = 0; i < n; ++i)
		{
			_vertexs.push_back(v[i]);
			_vertexsIndexMap[v[i]] = i;
		}
		_martexs.resize(_vertexs.size());
		for (size_t i = 0; i < _vertexs.size(); ++i)
		{
			_martexs[i].resize(_vertexs.size());
		}
	}

	size_t GetVertexIndex(const V& v)
	{
		if (_vertexsIndexMap.count(v))
		{
			return _vertexsIndexMap[v];
		}
		else
		{
			throw invalid_argument("顶点不存在");
		}
	}

	void AddEdge(const V& src, const V& dst, const W& w)
	{
		size_t srcindex = GetVertexIndex(src);
		size_t dstindex = GetVertexIndex(dst);

		_martexs[srcindex][dstindex] = w;
		if (Direction == false)
		{
			_martexs[dstindex][srcindex] = w;
		}
	}
private:
	vector<V> _vertexs;			//顶点集合
	map<V, size_t> _vertexsIndexMap;
	//W** _martexs;				//矩阵(需要动态开辟空间)
	vector<vector<W>> _martexs;   //矩阵
};

void TestGraphMartix()
{
	string strs[] = { "aaa", "bbb", "ccc", "ddd" };
	GraphMartix<string, int> gm(strs, sizeof(strs) / sizeof(strs[0]));

	gm.AddEdge("aaa", "bbb", 50);
	gm.AddEdge("aaa", "ddd", 60);
	gm.AddEdge("ddd", "aaa", 60);
	gm.AddEdge("ddd", "bbb", 100);
	gm.AddEdge("ccc", "bbb", 20);

}
