#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int N = 15;       //15*15������
const char ChessBoard = ' ';  //���̱�־
const char flag1 = 'o';    //���1����Ա�־
const char flag2 = 'x';    //���2��־

typedef struct Position{    //����
	int row;		//��
	int col;		//��
}Position;

class GoBang{     //��������
public:
	GoBang(){
		InitChessBoard();      //��ʼ������
	}
	void Play(){      //����
		Position Play1;   //���1�����
		Position Play2;   //���2
		while (1){
			int mode = ChoiceMode();
			while (1){
				if (mode == 1){       //����VS���
					ComputerChess(Play1, flag1);			//������
					if (GetVictory(Play1, 0, flag1)){		//0������ԣ�Ϊ�����ʾ���Ի�ʤ
						break;
					}
					PlayChess(Play2, 2, flag2);			//���2��
					if (GetVictory(Play2, 2, flag2)){		//2�������2
						break;
					}
				}
				else{				//���1VS���2
					PlayChess(Play1, 1, flag1);			//���1��
					if (GetVictory(Play1, 1, flag1)){		//���1Ӯ
						break;
					}
					PlayChess(Play2, 2, flag2);			//���2��
					if (GetVictory(Play2, 2, flag2)){		//���2Ӯ
						break;
					}
				}
			}
			cout << "======����һ��=======" << endl;
			cout << "yes or no :";	
			char s[] = "yes";
			cin >> s;
			if (s == "no"){
				break;
			}
		}
	}

protected:
	void InitChessBoard(){			//��ʼ������
		for (int i = 0; i < N + 1; ++i){
			for (int j = 0; j < N + 1; ++j){
				_ChessBoard[i][j] = ChessBoard;
			}
		}
	}

	int ChoiceMode(){			//ѡ��ģʽ
		system("cls");
		//ϵͳ���ã�����
		InitChessBoard();		//���³�ʼ������
		cout << "*************************************************" << endl;
		cout << "******************0���˳�************************" << endl;
		cout << "******************1������VS���******************" << endl;
		cout << "******************2�����VS���******************" << endl;
		cout << "*************************************************" << endl;
		while (1){
			int i = 0;
			cout << "��ѡ��ģʽ��";
			cin >> i;
			if (i == 0){       //�˳�
				exit(1);
			}
			if (i == 1 || i == 2){
				return i;
			}
			else{
				cout << "�Ƿ����룬���������룡" << endl;
			}
		}
	}

	void PrintChessBoard(){        //��ӡ����
		printf("     1   2   3   4   5   6   7   8   9  10  11  12  13  14  15\n");
		printf("   |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|\n");
		for (int i = 1; i < N + 1; ++i)
		{
			printf("%2d ", i);
			printf("| %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c |\n", _ChessBoard[i][1], _ChessBoard[i][2], _ChessBoard[i][3], _ChessBoard[i][4], _ChessBoard[i][5], _ChessBoard[i][6], _ChessBoard[i][7], _ChessBoard[i][8], _ChessBoard[i][9], _ChessBoard[i][10], _ChessBoard[i][11], _ChessBoard[i][12], _ChessBoard[i][13], _ChessBoard[i][14], _ChessBoard[i][15]);
			printf("   |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|\n");
		}
		cout << endl;
	}

	void ComputerChess(Position& pos, char flag){     //������
		//PrintChessBoard();      //��ӡ����
		int x = 0;
		int y = 0;
		while (1){                    //ѭ�����ҿ�λ��
			x = (rand() % N) + 1;      //������1~N�������
			srand((unsigned int)time(NULL));
			y = (rand() % N) + 1;      //������1~N�������
			srand((unsigned int)time(NULL));
			if (_ChessBoard[x][y] == ChessBoard){       //������λ��Ϊ�գ�û�����ӣ�������ѭ��������
				break;
			}
		}
		pos.row = x;
		pos.col = y;
		_ChessBoard[pos.row][pos.col] = flag;
	}

	void PlayChess(Position& pos, int player, char flag){
		PrintChessBoard();      //��ӡ����
		while (1){
			printf("�����%d�������꣺", player);
			cin >> pos.row >> pos.col;
			if (JudgeValue(pos) == 1){        //�ж������Ƿ�Ϸ�
				break;
			}
			cout << "���겻�Ϸ������������룺" << endl;
		}
		_ChessBoard[pos.row][pos.col] = flag;
	}

	int JudgeValue(const Position& pos){       //�ж�����ĺϷ���
		//1.��������
		if (pos.row > 0 && pos.row <= N && pos.col > 0 && pos.col <= N){
			//2.����λ��Ϊ�գ�û�����ӣ�
			if (_ChessBoard[pos.row][pos.col] == ChessBoard){
				return 1;      //�Ϸ�
			} 
		}
		return 0;       //�Ƿ�
	}

	int JudgeVictory(Position pos, char flag){     //�ж��Ƿ�����һ�ʤ���ײ��жϣ�
		int begin = 0;
		int end = 0;

		//1.�ж����Ƿ���������
		(pos.col - 4) > 0 ? begin = (pos.col - 4) : begin = 1;
		(pos.col + 4) > N ? end = N : end = (pos.col + 4);
		for (int i = pos.row, j = begin; j + 4 <= end; ++j){
			if (_ChessBoard[i][j] == flag && _ChessBoard[i][j + 1] == flag &&
				_ChessBoard[i][j + 2] == flag && _ChessBoard[i][j + 3] == flag &&
				_ChessBoard[i][j + 4] == flag)
				return 1;
		}
 		//2.�ж����Ƿ���������
		(pos.row - 4) > 0 ? begin = (pos.row - 4) : begin = 1;
		(pos.row + 4) > N ? end = N : end = (pos.row + 4);
		for (int j = pos.col, i = begin ; i + 4 <= end; ++i){
			if (_ChessBoard[i][j] == flag && _ChessBoard[i + 1][j] == flag &&
				_ChessBoard[i + 2][j] == flag && _ChessBoard[i + 3][j] == flag &&
				_ChessBoard[i + 4][j] == flag)
				return 1;
		}
		//3.�ж����Խ����Ƿ���������
		int len = 0;    //��Գ���
		int start = 0;
		int finish = 0;
		pos.row > pos.col ? len = pos.col - 1 : len = pos.row - 1;
		if (len > 4){
			len = 4;
		}
		begin = pos.row - len;       //��������ʼλ��
		start = pos.col - len;       //��������ʼλ��
		  
		pos.row > pos.col ? len = N - pos.row : len = N - pos.col;
		if (len > 4){
			len = 4;
		}
		end = pos.row + len;         //���������λ��
		finish = pos.col + len;      //���������λ��
	     
		for (int i = begin, j = start; (i + 4 <= end) && (j + 4 <= finish); ++i, ++j){
			if (_ChessBoard[i][j] == flag && _ChessBoard[i + 1][j + 1] == flag &&
				_ChessBoard[i + 2][j + 2] == flag && _ChessBoard[i + 3][j + 3] == flag &&
				_ChessBoard[i + 4][j + 4] == flag)
				return 1;
		}
		//4.�жϸ��Խ����Ƿ���������
		(pos.row - 1) > (N - pos.col) ? len = N - pos.col : len = pos.row - 1;
		if (len > 4){
			len = 4;
		}
		begin = pos.row - len;       //��������ʼλ��
		start = pos.col + len;       //��������ʼλ��

		(N - pos.row) > (pos.col - 1) ? len = pos.col - 1 : len = N - pos.row;
		if (len > 4){
			len = 4;
		}
		end = pos.row + len;         //���������λ��
		finish = pos.col - len;      //���������λ�� 
		for (int i = begin, j = start; (i + 4 <= end) && (j - 4 >= finish); ++i, --j){
			if (_ChessBoard[i][j] == flag && _ChessBoard[i + 1][j - 1] == flag &&
				_ChessBoard[i + 2][j - 2] == flag && _ChessBoard[i + 3][j - 3] == flag &&
				_ChessBoard[i + 4][j - 4] == flag)
				return 1;
		}
		//��λ�ò�δ����
		for (int x = 1; x < N + 1; ++x){
			for (int y = 1; y < N + 1; ++y){
				if (_ChessBoard[x][y] == ChessBoard){
					return 0;       //δ����
				}
			}
		}
		return -1;      //�;�
	}

	bool GetVictory(Position& pos, int player, char flag){       //�жϾ�����λ���Ӯ
		if (JudgeVictory(pos, flag) != 0){    //�ж������˻�ʤ
			if (JudgeVictory(pos, flag) == 1){     //�ж��Ƿ����˻�ʤ,1��ʾ��ʤ
				PrintChessBoard();     //��ӡ����
				if (player == 0){
					cout << "���Ի�ʤ��" << endl;
				}
				else{
					printf("��ϲ���%d��ʤ��\n", player);
				}
			}
			else{
				printf("�;֣�\n");
			}
			return true;      //���˻�ʤ
		}	
		return false;         //û�˻�ʤ
	}
		
private:
	char _ChessBoard[N + 1][N + 1];    //����
};

int main(){
	GoBang g;
	g.Play();
	system("pause");
	return 0;
}
