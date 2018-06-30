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

//	//压缩
//	void Compress(const char* file)
//	{
//		//1.统计字符出现的次数
//		FILE* fout = fopen(file, "r");
//		assert(fout);
//
//		char ch = fgetc(fout);
//		while (ch != EOF)
//		{
//			_infos[ch]._count++;
//			ch = fgetc(fout);
//		}
//
//		//2.生成huffman tree 及 huffman code
//		CharInfo invalid;
//		invalid._count = 0;
//		HuffmanTree<CharInfo> tree(_infos, 256, invalid);
//
//		//得到压缩文件名
//		string compressfile = file;
//		compressfile += ".huffman";
//
//		//打开压缩文件
//		FILE* fin = fopen(compressfile.c_str(), "w");
//		assert(fin);
//
//		string code;
//		GenerateHuffmanCode(tree.GetRoot(), code);
//
//		//3.压缩
//		fseek(fout, 0, SEEK_SET);
//		ch = fgetc(fout);
//		//value不能动，只能移动某一位pos
//		char value = 0;
//		size_t pos = 0;
//		while (ch != EOF)
//		{
//			//string的拷贝构造代价太大
//			string& code = _infos[ch]._code;
//			//读取一个字符的编码
//			for (size_t i = 0; i < code.size(); ++i)
//			{
//				//左移是向高位移
//				if (code[i] == '1')
//				{
//					value |= (1 << pos);
//				}
//				else if (code[i] == '0')
//				{
//					value &= ~(1 << pos);
//				}
//				else
//				{
//					assert(false);
//					//throw exception("invalid huffman code");
//				}
//				++pos;
//				if (pos == 8)
//				{
//					fputc(value, fin);
//					printf("0x%x ", value);
//					value = 0;
//					pos = 0;
//				}
//			}
//
//			//读取下一个字符编码
//			ch = fgetc(fout);
//		}
//		//不满8位时
//		if (pos > 0)
//		{
//			fputc(value, fin);
//			printf("0x%x ", value);
//		}
//		fclose(fout);
//		fclose(fin);
//	}
//
//	//解压缩
//	void UnCompress(const char* file)
//	{
//		CharInfo invalid;
//		invalid._count = 0;
//		HuffmanTree<CharInfo> tree(_infos, 256, invalid);
//		Node* root = tree.GetRoot();
//		LongType count = root->_w._count;
//
//		string unfile = file;
//		size_t pos = unfile.find('.');
//		if (pos != string::npos)
//		{
//			unfile.erase(pos);
//		}
//
//#ifdef _DEBUG
//		unfile += ".unhuffman";
//#endif
//
//		//打开压缩文件
//		ifstream ifs(file);
//		ofstream ofs(unfile.c_str());
//		char value;
//		Node* cur = root;
//		while (ifs.get(value))
//		{
//			for (size_t i = 0; i < 8; ++i)
//			{
//				if (value & (1 << i))
//				{
//					cur = cur->_right;
//				}
//				else
//				{
//					cur = cur->_left;
//				}
//				if (cur->_left == NULL && cur->_right == NULL)
//				{
//					ofs.put(cur->_w._ch);
//					cur = root;
//					if (--count == 0)
//					{
//						return;
//					}
//				}
//			}
//		}
//	}

public:
	const string Compress(string filename)
	{
		vector<string> file;
		string path = filename.c_str();
		GetFiles(path, file);

		if (file.empty())     //如果为空，则表示是一个文件
		{
			return _Compress(filename);
		}
		else                        //一个文件夹
		{
			//首先创建一个新的文件夹
			string newpath = path;               //新的压缩后文件夹的路径名
			newpath += ".huffman";
			_mkdir(newpath.c_str());   //

			for (int i = 0; i < (int)file.size(); i++)
			{
				_Compress(file[i], newpath);
			}
			return newpath;          //返回新建的压缩文件夹的名字
		}
	}

	const string UnCompress(string filename)
	{
		vector<string> file;
		string path = filename.c_str();
		GetFiles(path, file);

		if (file.empty())     //如果为空，则表示是一个文件进行解压缩
		{
			return _UnCompress(filename);
		}
		else                        //一个文件夹进行解压缩
		{
			//首先创建一个新的文件夹
			string newpath = filename;               //新文件夹
			for (int i = (int)filename.size() - 1; i >= 0; i--)
			{
				if (filename[i] == '.')
				{
					newpath.resize(i);
					break;
				}
			}
			newpath += ".unhuffman";                  //新文件夹的路径名
			_mkdir(newpath.c_str());             //创建一个新的解压缩的文件夹

			for (int i = 0; i < (int)file.size(); i++)
			{
				_UnCompress(file[i], newpath);
			}
			return newpath;          //返回新建的文件夹
		}
	}


protected:
	//文件压缩
	const string _Compress(const string file, const string path = string())
	{
		//1.统计字符出现的次数
		FILE* fout = fopen(file.c_str(), "r");
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
		string code;
		GenerateHuffmanCode(tree.GetRoot(), code);

		//得到压缩文件名
		string compressfile = file;
		compressfile += ".huffman";

		//得到配置文件名
		string configFile = file;
		configFile += ".config";

		FILE* fin = NULL;
		FILE* configIn = NULL;

		if (path.empty())        //路径为空，表示为但文本文件
		{
			//打开压缩文件
			fin = fopen(compressfile.c_str(), "wb");
			assert(fin);
			//编写配置文件，保存字符以及字符出现的次数，用于解压时重建huffman tree
			configIn = fopen(configFile.c_str(), "wb");
			assert(configIn);
		}
		else                  //路径不为空，表示为文件夹
		{
			//得到要创建的路径
			string newfile;   //得到新的文件名
			int i = file.size() - 1;
			for (; i >= 0; --i)
			{
				if (file[i] == '\\')
					break;
				else
					newfile += file[i];
			}
			reverse(newfile.begin(), newfile.end());
			string newpath = path;
			newpath += '\\';
			newpath += newfile;
			newpath += ".huffman";
			//打开压缩文件
			fin = fopen(compressfile.c_str(), "wb");
			assert(fin);
			//编写配置文件，保存字符以及字符出现的次数，用于解压时重建huffman tree
			newpath = path;
			newpath += '\\';
			newpath += newfile;
			newpath += ".config";
			configIn = fopen(newpath.c_str(), "wb");    
			assert(configIn);
		}
		//编写配置文件
		string line;
		for (int i = 0; i < 256; ++i)
		{
			//将字符及字符出现的次数存一行
			if (_infos[i]._count != 0)
			{
				int c = _infos[i]._ch;
				fputc(c, configIn);
				line += ",";
				//将次数转为字符串存储
				char buf[30] = { 0 };
				line += _itoa((int)_infos[i]._count, buf, 10);
				line += '\n';
				fwrite(line.c_str(), 1, line.size(), configIn);
				line.clear();
			}
		}
		fclose(configIn);      //配置文件编写完成后关闭配置文件

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
					//printf("0x%x ", value);
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
			//printf("0x%x ", value);
		}
		fclose(fout);
		fclose(fin);
		memset(_infos, 0, 256 * sizeof(CharInfo));
		return compressfile;
	}

	//解压缩
	const string _UnCompress(const string file, const string path = string())
	{
		assert(!file.empty());

		//得到解压缩之后的文件名
		int i = 0;
		string cur;
		for (i = (int)file.size() - 1; i >= 0; --i)
		{
			cur.push_back(file[i]);
			if (file[i] == '.')
				break;
		}
		reverse(cur.begin(), cur.end());    //pos保存需要解压文件的后缀
		if (cur != ".huffman")
		{
			return string();
		}
		//去掉后缀
		string unfile = file;
		size_t pos = unfile.find('.');
		if (pos != string::npos)
		{
			unfile.erase(pos);
		}
		//得到配置文件名
		string configFile = unfile;
		configFile += ".txt.config";

#ifdef _DEBUG
		unfile += ".unhuffman";
#endif
		
		//打开压缩文件
		FILE* fout = fopen(file.c_str(), "rb");
		assert(fout);
		//打开配置文件
		FILE* configOut = fopen(configFile.c_str(), "rb");
		assert(configOut);

		FILE* fin = NULL;
		if (path.empty())        //路径为空，表示为但文本文件
		{
			//打开压缩文件
			fin = fopen(unfile.c_str(), "wb");
			assert(fin);
		}
		else                  //路径不为空，表示为文件夹
		{
			string newfile;   //得到压缩的文件名
			string filename = file;
			size_t pos = filename.find('.');
			if (pos != string::npos)
			{
				filename.erase(pos);
			}
			int i = filename.size() - 1;
			for (; i >= 0; --i)
			{
				if (filename[i] == '\\')
					break;
				else
					newfile += filename[i];
			}
			reverse(newfile.begin(), newfile.end());
			string newpath = path;
			newpath += '\\';
			newpath += newfile;
			newpath += ".unhuffman";
			//打开解压缩文件
			fin = fopen(newpath.c_str(), "w");
			assert(fin);
		}
		//编写配置文件
		string line;
		int c = 0;
		while (c = fgetc(configOut) != EOF)   //读取一行
		{
			GetLine(configOut, line);
			_infos[c]._count = atoi(&line[1]);     //获取字符出现次数
			line.clear();
		}
		fclose(configOut);      //配置文件编写完成后关闭配置文件

		//构建huffman树
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);
		Node* root = tree.GetRoot();
		LongType count = root->_w._count;
		int index = 7;
		int ch = 0;
		Node* curr = root;
		while (count > 0)         //以字符串总个数为结束标志
		{
			ch = fgetc(fout);
			while (index >= 0 && count > 0)
			{
				if (ch & (1 << index))
				{
					curr = curr->_right;
				}
				else
				{
					curr = curr->_left;
				}
				if (curr->_left == NULL && curr->_right == NULL)
				{
					fputc(curr->_w._ch, fin);
					curr = root;
					--count;
				}
				index--;
			}
			index = 7;
		}
		fclose(fout);
		fclose(fin);
		memset(_infos, 0, 256 * sizeof(CharInfo));
		return unfile;
	}

	bool GetLine(FILE*& output, string& line)
	{
		assert(output);
		int ch = 0;
		ch = fgetc(output);
		if (ch == EOF)
			return false;
		while (ch != EOF && ch != '\n')
		{
			line += ch;
			ch = fgetc(output);
		}
		return true;
	}

	void GetFiles(string path, vector<string>& files)
	{
		//文件句柄
		long   hFile = 0;
		//文件信息
		struct _finddata_t fileinfo;
		string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		{
			do
			{
				//如果是目录,迭代之
				//如果不是,加入列表
				if ((fileinfo.attrib &  _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						GetFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
				else
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
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
