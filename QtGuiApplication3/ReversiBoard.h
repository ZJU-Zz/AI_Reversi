#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <windows.h>
#include "LoadWriteNodes.h"

using namespace std;
///enum Type
//{
//	EMPTY = 0,
//	BLACK = 1,
//	WHITE = 2
//};


static int direction[8][2] = {
	{ -1,-1 }, { -1,0 },	{ -1,1 },
	{ 0 ,-1  },				{ 0,1  },
	{ 1,-1  }, { 1,0  },	{ 1,1  }
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


