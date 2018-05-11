#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <windows.h>
#include "LoadWriteNodes.h"

using namespace std;

static int direction[8][2] = {	//��������
	{ -1,-1 }, { -1,0 },	{ -1,1 },
	{ 0 ,-1  },				{ 0,1  },
	{ 1,-1  }, { 1,0  },	{ 1,1  }
};


static int smartDirection[60][2] = {		//�������ȼ������λ��
	{0,0},{7,0},{0,7},{7,7},
{0,2},{0,5},{2,0},{2,7},{5,0},{5,7},{7,2},{7,5},
{7,3},{7,4},{0,3},{0,4},{6,2},{6,5},{1,2},{1,5},{2,1},{2,6},{5,1},{5,6},{3,0},{3,7},{4,0},{4,7},
{2,2},{2,5},{5,2},{5,5},
{3,2},{3,5},{4,2},{4,5},{2,3},{2,4},{5,3},{5,4},
{1,3},{1,4},{6,3},{6,4},{3,1},{3,6},{4,1},{4,6},
{1,0},{1,1},{0,1},{0,6},{1,7},{1,6},{6,0},{6,1},{7,1},{7,6},{6,6},{6,7}
};




class ReversiBoard
{
public:
	ReversiBoard();
	ReversiBoard(TreeNode n);	//�ýڵ��ʼ������
	~ReversiBoard();
	Type getType(int x, int y);
	void setType(Type t,int x, int y);
	vector<int> canPlay(int x, int y);
	void doPlay(vector<int> dir,int x, int y);
	bool isValid(int x, int y);		//�����Ƿ���Ч
	void printBoard();			//�ڿ���̨��ӡ����
	void reset();			//��ʼ��
	Type getPlaying();		//�õ������µ�һ��
	void reversePlaying();	//�ı������µ��ҷ�

	void takeBack();		//��һ����

	int getCount(Type p);	//ͳ��ĳ�����ӵ�����

	void autoRandomPlayOneStep();	//�����һ��
	Type autoRandomPlay();	//�����֪������

	bool isLeaf();	//�ж��Ƿ��Ѿ�����

	TreeNode getLeaf(vector<int> dir,int x,int y);	//���ı䵱ǰ���̣��õ���ĳһ��֮�������

	bool isEnd();	//�ж��Ƿ����
	unsigned short int* getBoard();		//��ȡBoard
	int count = 4;
private:
	Type playing;
	unsigned short int Board[8];
	unsigned short int lastBoard[8];	//�û�����
	int end = 0;
	bool canRegret = false;	
};


