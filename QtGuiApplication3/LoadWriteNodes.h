#pragma once
#include"TreeNode.h"
#include<fstream>
using namespace std;

class LoadWriteNodes
{
public:
	LoadWriteNodes();
	~LoadWriteNodes();
	unordered_set<TreeNode, HashFunc, EqualKey> Nodes;
	unordered_set<TreeNode, HashFunc, EqualKey>::iterator iter;

	void printHash();
private:
	string FileName = "TreeNode.data";
};


