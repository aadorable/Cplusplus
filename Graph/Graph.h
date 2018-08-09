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

/////////////////////////////////////////////////////////////////////////////////////////////////
//临接表
/////////////////////////////////////////////////////////////////////////////////////////////////
template<class W>
struct Edge
{
	Edge<W>* _next;
	size_t srcindex;
	size_t dstindex;
	W _w;
};

template<class V, class W, bool Direction = false>
class GraphLink
{
	typedef Edge<W> Edge;
public:
	GraphLink(V* v, size_t n)
	{
		_vertexs.reserve(n);
		for (size_t i = 0; i < n; ++i)
		{
			_vertexs.push_back(v[i]);
			_vertexsIndexMap[v[i]] = i;
		}
		_linktable.resize(_vertexs.size(), NULL);
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

	void _AddEdge(size_t srcindex, size_t dstindex, const W& w)
	{
		Edge* edge = new Edge;
		edge->srcindex = srcindex;
		edge->dstindex = dstindex;
		edge->_w = w;
		edge->_next = NULL;

		edge->_next = _linktable[srcindex];
		_linktable[srcindex] = edge;
	}


	void AddEdge(const V& src, const V& dst, const W& w)
	{
		size_t srcindex = GetVertexIndex(src);
		size_t dstindex = GetVertexIndex(dst);

		_AddEdge(srcindex, dstindex, w);
		if (Direction == false)
		{
			_AddEdge(dstindex, srcindex, w);
		}
	}

	void GFS(const V& src)      //广度优先遍历
	{
		size_t srcindex = GetVertexIndex(src);
		queue<size_t> q;
		q.push(srcindex);

		vector<bool> visited;
		visited.resize(_vertexs.size(), false);
		while (!q.empty())
		{
			size_t front = q.front();
			q.pop();
			if (visited[front] == false)
			{
				cout << _vertexs[front] << ": " << front << endl;
				visited[front] = true;

				Edge* edge = _linktable[front];
				while (edge)
				{
					q.push(edge->dstindex);
					edge = edge->_next;
				}
			}
		}
		cout << endl;
	}

	void _DFS(size_t srcindex, vector<bool> visited)
	{
		cout << _vertexs[srcindex] << ": " << srcindex << endl; 
		visited[srcindex] = true;
		Edge* edge = _linktable[srcindex];
		while (edge)
		{
			if (visited[edge->dstindex] == false)
			{
				_DFS(edge->dstindex, visited);
			}
			edge = edge->_next;
		}
	}

	void DFS(const V& src)      //深度优先遍历
	{
		size_t srcindex = GetVertexIndex(src);
		vector<bool> visited;
		visited.resize(_vertexs.size(), false);
		_DFS(srcindex, visited);
		cout << endl;
	}

private:
	vector<V> _vertexs;			//顶点集合
	map<V, size_t> _vertexsIndexMap;

	vector<Edge*> _linktable;       //临接表
};

void TestGraphLink()
{
	string strs[] = { "aaa", "bbb", "ccc", "ddd" };
	GraphLink<string, int> gl(strs, sizeof(strs) / sizeof(strs[0]));

	gl.AddEdge("aaa", "bbb", 50);
	gl.AddEdge("aaa", "ddd", 60);
	gl.AddEdge("ddd", "aaa", 60);
	gl.AddEdge("ddd", "bbb", 100);
	gl.AddEdge("ccc", "bbb", 20);
	//gl.GFS("aaa");
	gl.DFS("aaa");
}
