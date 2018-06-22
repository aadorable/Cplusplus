#pragma once

#include <assert.h>
#include <string>
#include <fstream>
#include "Huffman.h"

using namespace std;

typedef long long LongType;

struct CharInfo
{
	char _ch;
	LongType _count;
	string _code;

	//w[i] != invalid 自定义类型的比较要进行运算符重载
	bool operator!=(const CharInfo& info)
	{
		return _count != info._count;
	}

	//left->_w + right->_w 
	CharInfo operator+(const CharInfo& info)
	{
		CharInfo ret;
		ret._count = _count + info._count;
		return ret;
	}

	//l->_w < r->_w
	bool operator<(const CharInfo& info)
	{
		return _count < info._count;
	}
};

//单文件版本,不支持文件夹压缩
class FileCompress
{
	typedef HuffmanTreeNode<CharInfo> Node;
public:
	FileCompress()
	{
		//将Infos初始化
		for (size_t i = 0; i < 256; ++i)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
			//string会调用缺省构造函数(空字符串)
		}
	}

	void GenerateHuffmanCode(Node* root, string code)
	{
		if (root == NULL)
		{
			return;
		}
		if (root->_left == NULL && root->_right == NULL)
		{
			_infos[root->_w._ch]._code = code;
			return;
		}
		GenerateHuffmanCode(root->_left, code+'0');
		GenerateHuffmanCode(root->_right, code+'1');
	}

	//压缩
	void Compress(const char* file)
	{
		//1.统计字符出现的次数
		FILE* fout = fopen(file, "r");
		assert(fout);

		char ch = fgetc(fout);
		while (ch != EOF)
		{
			_infos[ch]._count++;
			ch = fgetc(fout);
		}

		//2.生成huffman tree 及 huffman code
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);

		//得到压缩文件名
		string compressfile = file;
		compressfile += ".huffman";

		//打开压缩文件
		FILE* fin = fopen(compressfile.c_str(), "w");
		assert(fin);

		string code;
		GenerateHuffmanCode(tree.GetRoot(), code);

		//3.压缩
		fseek(fout, 0, SEEK_SET);
		ch = fgetc(fout);
		//value不能动，只能移动某一位pos
		char value = 0;
		size_t pos = 0;
		while (ch != EOF)
		{
			//string的拷贝构造代价太大
			string& code = _infos[ch]._code;
			//读取一个字符的编码
			for (size_t i = 0; i < code.size(); ++i)
			{
				//左移是向高位移
				if (code[i] == '1')
				{
					value |= (1 << pos);
				}
				else if (code[i] == '0')
				{
					value &= ~(1 << pos);
				}
				else
				{
					assert(false);
					//throw exception("invalid huffman code");
				}
				++pos;
				if (pos == 8)
				{
					fputc(value, fin);
					printf("0x%x ", value);
					value = 0;
					pos = 0;
				}
			}

			//读取下一个字符编码
			ch = fgetc(fout);
		}
		//不满8位时
		if (pos > 0)
		{
			fputc(value, fin);
			printf("0x%x ", value);
		}
		fclose(fout);
		fclose(fin);
	}

	//解压缩
	void UnCompress(const char* file)
	{
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);
		Node* root = tree.GetRoot();
		LongType count = root->_w._count;

		string unfile = file;
		size_t pos = unfile.find('.');
		if (pos != string::npos)
		{
			unfile.erase(pos);
		}

#ifdef _DEBUG
		unfile += ".unhuffman";
#endif

		//打开压缩文件
		ifstream ifs(file);
		ofstream ofs(unfile.c_str());
		char value;
		Node* cur = root;
		while (ifs.get(value))
		{
			for (size_t i = 0; i < 8; ++i)
			{
				if (value & (1 << i))
				{
					cur = cur->_right;
				}
				else
				{
					cur = cur->_left;
				}
				if (cur->_left == NULL && cur->_right == NULL)
				{
					ofs.put(cur->_w._ch);
					cur = root;
					if (--count == 0)
					{
						return;
					}
				}
			}
		}
	}
protected:
	CharInfo _infos[256];
};

void TestFileCompress()
{
	FileCompress fc;
	fc.Compress("Input.txt");
	fc.UnCompress("Input.txt.huffman");
}