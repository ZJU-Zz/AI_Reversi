#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <windows.h>
#include "LoadWriteNodes.h"

using namespace std;

static int direction[8][2] = {	//方向向量
	{ -1,-1 }, { -1,0 },	{ -1,1 },
	{ 0 ,-1  },				{ 0,1  },
	{ 1,-1  }, { 1,0  },	{ 1,1  }
};


static int smartDirection[60][2] = {		//按照优先级排序的位置
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
	ReversiBoard(TreeNode n);	//用节点初始化棋盘
	~ReversiBoard();
	Type getType(int x, int y);
	void setType(Type t,int x, int y);
	vector<int> canPlay(int x, int y);
	void doPlay(vector<int> dir,int x, int y);
	bool isValid(int x, int y);		//坐标是否有效
	void printBoard();			//在控制台打印棋盘
	void reset();			//初始化
	Type getPlaying();		//得到正在下的一方
	void reversePlaying();	//改变正在下的乙方

	void takeBack();		//悔一步棋

	int getCount(Type p);	//统计某类棋子的数量

	void autoRandomPlayOneStep();	//随机下一步
	Type autoRandomPlay();	//随机下知道结束

	bool isLeaf();	//判断是否已经结束

	TreeNode getLeaf(vector<int> dir,int x,int y);	//不改变当前棋盘，得到下某一步之后的棋面

	bool isEnd();	//判断是否结束
	unsigned short int* getBoard();		//获取Board
	int count = 4;
private:
	Type playing;
	unsigned short int Board[8];
	unsigned short int lastBoard[8];	//用户悔棋
	int end = 0;
	bool canRegret = false;	
};


