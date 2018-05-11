#pragma once
#include"TreeNode.h"
#include<fstream>
#include "ReversiBoard.h"
#include <QThread>
#include <QTime>
#include <QReadWriteLock>
using namespace std;

typedef struct point Point;
struct point
{
	int x;
	int y;
};


class LoadWriteNodes : public QThread
{
public:
	LoadWriteNodes(unsigned short int* Board,Type playing,int count);
	LoadWriteNodes();
	~LoadWriteNodes();
	unordered_set<TreeNode, HashFunc, EqualKey> Nodes;
	unordered_set<TreeNode, HashFunc, EqualKey>::iterator iter;

	virtual void run();
	void Search(TreeNode next);
	void backUp(TreeNode next, Type win, int count);
	Point getBestChild(unsigned int total, double value[][8], unsigned int playTime[][8]);

	void changeCurrent(unsigned short int* Board, Type playing, int count);
	void printHash();
private:
	string FileName = "TreeNode.data";
	double C = sqrt(2);
	unsigned short int* Board;
	Type playing;
	int c_count;
	bool need_store = false;
};


