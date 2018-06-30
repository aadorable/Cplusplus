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

	//w[i] != invalid �Զ������͵ıȽ�Ҫ�������������
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

//���ļ��汾,��֧���ļ���ѹ��
class FileCompress
{
	typedef HuffmanTreeNode<CharInfo> Node;
public:
	FileCompress()
	{
		//��Infos��ʼ��
		for (size_t i = 0; i < 256; ++i)
		{
			_infos[i]._ch = i;
			_infos[i]._count = 0;
			//string�����ȱʡ���캯��(���ַ���)
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

//	//ѹ��
//	void Compress(const char* file)
//	{
//		//1.ͳ���ַ����ֵĴ���
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
//		//2.����huffman tree �� huffman code
//		CharInfo invalid;
//		invalid._count = 0;
//		HuffmanTree<CharInfo> tree(_infos, 256, invalid);
//
//		//�õ�ѹ���ļ���
//		string compressfile = file;
//		compressfile += ".huffman";
//
//		//��ѹ���ļ�
//		FILE* fin = fopen(compressfile.c_str(), "w");
//		assert(fin);
//
//		string code;
//		GenerateHuffmanCode(tree.GetRoot(), code);
//
//		//3.ѹ��
//		fseek(fout, 0, SEEK_SET);
//		ch = fgetc(fout);
//		//value���ܶ���ֻ���ƶ�ĳһλpos
//		char value = 0;
//		size_t pos = 0;
//		while (ch != EOF)
//		{
//			//string�Ŀ����������̫��
//			string& code = _infos[ch]._code;
//			//��ȡһ���ַ��ı���
//			for (size_t i = 0; i < code.size(); ++i)
//			{
//				//���������λ��
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
//			//��ȡ��һ���ַ�����
//			ch = fgetc(fout);
//		}
//		//����8λʱ
//		if (pos > 0)
//		{
//			fputc(value, fin);
//			printf("0x%x ", value);
//		}
//		fclose(fout);
//		fclose(fin);
//	}
//
//	//��ѹ��
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
//		//��ѹ���ļ�
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

		if (file.empty())     //���Ϊ�գ����ʾ��һ���ļ�
		{
			return _Compress(filename);
		}
		else                        //һ���ļ���
		{
			//���ȴ���һ���µ��ļ���
			string newpath = path;               //�µ�ѹ�����ļ��е�·����
			newpath += ".huffman";
			_mkdir(newpath.c_str());   //

			for (int i = 0; i < (int)file.size(); i++)
			{
				_Compress(file[i], newpath);
			}
			return newpath;          //�����½���ѹ���ļ��е�����
		}
	}

	const string UnCompress(string filename)
	{
		vector<string> file;
		string path = filename.c_str();
		GetFiles(path, file);

		if (file.empty())     //���Ϊ�գ����ʾ��һ���ļ����н�ѹ��
		{
			return _UnCompress(filename);
		}
		else                        //һ���ļ��н��н�ѹ��
		{
			//���ȴ���һ���µ��ļ���
			string newpath = filename;               //���ļ���
			for (int i = (int)filename.size() - 1; i >= 0; i--)
			{
				if (filename[i] == '.')
				{
					newpath.resize(i);
					break;
				}
			}
			newpath += ".unhuffman";                  //���ļ��е�·����
			_mkdir(newpath.c_str());             //����һ���µĽ�ѹ�����ļ���

			for (int i = 0; i < (int)file.size(); i++)
			{
				_UnCompress(file[i], newpath);
			}
			return newpath;          //�����½����ļ���
		}
	}


protected:
	//�ļ�ѹ��
	const string _Compress(const string file, const string path = string())
	{
		//1.ͳ���ַ����ֵĴ���
		FILE* fout = fopen(file.c_str(), "r");
		assert(fout);

		char ch = fgetc(fout);
		while (ch != EOF)
		{
			_infos[ch]._count++;
			ch = fgetc(fout);
		}

		//2.����huffman tree �� huffman code
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);
		string code;
		GenerateHuffmanCode(tree.GetRoot(), code);

		//�õ�ѹ���ļ���
		string compressfile = file;
		compressfile += ".huffman";

		//�õ������ļ���
		string configFile = file;
		configFile += ".config";

		FILE* fin = NULL;
		FILE* configIn = NULL;

		if (path.empty())        //·��Ϊ�գ���ʾΪ���ı��ļ�
		{
			//��ѹ���ļ�
			fin = fopen(compressfile.c_str(), "wb");
			assert(fin);
			//��д�����ļ��������ַ��Լ��ַ����ֵĴ��������ڽ�ѹʱ�ؽ�huffman tree
			configIn = fopen(configFile.c_str(), "wb");
			assert(configIn);
		}
		else                  //·����Ϊ�գ���ʾΪ�ļ���
		{
			//�õ�Ҫ������·��
			string newfile;   //�õ��µ��ļ���
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
			//��ѹ���ļ�
			fin = fopen(compressfile.c_str(), "wb");
			assert(fin);
			//��д�����ļ��������ַ��Լ��ַ����ֵĴ��������ڽ�ѹʱ�ؽ�huffman tree
			newpath = path;
			newpath += '\\';
			newpath += newfile;
			newpath += ".config";
			configIn = fopen(newpath.c_str(), "wb");    
			assert(configIn);
		}
		//��д�����ļ�
		string line;
		for (int i = 0; i < 256; ++i)
		{
			//���ַ����ַ����ֵĴ�����һ��
			if (_infos[i]._count != 0)
			{
				int c = _infos[i]._ch;
				fputc(c, configIn);
				line += ",";
				//������תΪ�ַ����洢
				char buf[30] = { 0 };
				line += _itoa((int)_infos[i]._count, buf, 10);
				line += '\n';
				fwrite(line.c_str(), 1, line.size(), configIn);
				line.clear();
			}
		}
		fclose(configIn);      //�����ļ���д��ɺ�ر������ļ�

		//3.ѹ��
		fseek(fout, 0, SEEK_SET);
		ch = fgetc(fout);
		//value���ܶ���ֻ���ƶ�ĳһλpos
		char value = 0;
		size_t pos = 0;
		while (ch != EOF)
		{
			//string�Ŀ����������̫��
			string& code = _infos[ch]._code;
			//��ȡһ���ַ��ı���
			for (size_t i = 0; i < code.size(); ++i)
			{
				//���������λ��
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

			//��ȡ��һ���ַ�����
			ch = fgetc(fout);
		}
		//����8λʱ
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

	//��ѹ��
	const string _UnCompress(const string file, const string path = string())
	{
		assert(!file.empty());

		//�õ���ѹ��֮����ļ���
		int i = 0;
		string cur;
		for (i = (int)file.size() - 1; i >= 0; --i)
		{
			cur.push_back(file[i]);
			if (file[i] == '.')
				break;
		}
		reverse(cur.begin(), cur.end());    //pos������Ҫ��ѹ�ļ��ĺ�׺
		if (cur != ".huffman")
		{
			return string();
		}
		//ȥ����׺
		string unfile = file;
		size_t pos = unfile.find('.');
		if (pos != string::npos)
		{
			unfile.erase(pos);
		}
		//�õ������ļ���
		string configFile = unfile;
		configFile += ".txt.config";

#ifdef _DEBUG
		unfile += ".unhuffman";
#endif
		
		//��ѹ���ļ�
		FILE* fout = fopen(file.c_str(), "rb");
		assert(fout);
		//�������ļ�
		FILE* configOut = fopen(configFile.c_str(), "rb");
		assert(configOut);

		FILE* fin = NULL;
		if (path.empty())        //·��Ϊ�գ���ʾΪ���ı��ļ�
		{
			//��ѹ���ļ�
			fin = fopen(unfile.c_str(), "wb");
			assert(fin);
		}
		else                  //·����Ϊ�գ���ʾΪ�ļ���
		{
			string newfile;   //�õ�ѹ�����ļ���
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
			//�򿪽�ѹ���ļ�
			fin = fopen(newpath.c_str(), "w");
			assert(fin);
		}
		//��д�����ļ�
		string line;
		int c = 0;
		while (c = fgetc(configOut) != EOF)   //��ȡһ��
		{
			GetLine(configOut, line);
			_infos[c]._count = atoi(&line[1]);     //��ȡ�ַ����ִ���
			line.clear();
		}
		fclose(configOut);      //�����ļ���д��ɺ�ر������ļ�

		//����huffman��
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_infos, 256, invalid);
		Node* root = tree.GetRoot();
		LongType count = root->_w._count;
		int index = 7;
		int ch = 0;
		Node* curr = root;
		while (count > 0)         //���ַ����ܸ���Ϊ������־
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
		//�ļ����
		long   hFile = 0;
		//�ļ���Ϣ
		struct _finddata_t fileinfo;
		string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		{
			do
			{
				//�����Ŀ¼,����֮
				//�������,�����б�
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
