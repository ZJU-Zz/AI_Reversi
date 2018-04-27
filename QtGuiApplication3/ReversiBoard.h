#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <windows.h>
#include "LoadWriteNodes.h"

using namespace std;

static int direction[8][2] = {
	{ -1,-1 }, { -1,0 },	{ -1,1 },
	{ 0 ,-1  },				{ 0,1  },
	{ 1,-1  }, { 1,0  },	{ 1,1  }
};


static int smartDirection[60][2] = {
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
	ReversiBoard(TreeNode n);
	~ReversiBoard();
	Type getType(int x, int y);
	bool setType(Type t,int x, int y);
	vector<int> canPlay(int x, int y);
	void doPlay(vector<int> dir,int x, int y);
	bool isValid(int x, int y);
	void printBoard();
	void reset();
	Type getPlaying();
	void reversePlaying();

	void takeBack();

	int getCount(Type p);

	void autoRandomPlayOneStep();
	Type autoRandomPlay();

	bool isLeaf();

	TreeNode getLeaf(vector<int> dir,int x,int y);

	bool isEnd();
	unsigned short int* getBoard();
	int count = 4;
private:
	Type playing;
	unsigned short int Board[8];
	unsigned short int lastBoard[8];
	int end = 0;
	bool canRegret = false;
};


