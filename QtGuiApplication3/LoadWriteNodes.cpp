#include "LoadWriteNodes.h"

LoadWriteNodes::LoadWriteNodes()
{
	
	ifstream in = ifstream(FileName, std::ios::binary);
	if (!in.is_open())
	{
		TreeNode root;
		root.n = root.w = root.parentIndex = 0;
		root.playing = BLACK;
		for (int i = 0; i < 8; i++)
		{
			root.Board[i] = 0;
		}
		root.Board[3] = 0x240;
		root.Board[4] = 0x180;
		root.parentBoard[0] = 0xFFFF;
		Nodes.insert(root);
		return;
	}
	while (!in.eof())
	{
		TreeNode temp;
		in.read((char*)&temp, sizeof(temp));
		Nodes.insert(temp);
	}
	in.close();
	
}

LoadWriteNodes::~LoadWriteNodes()
{
	ofstream out = ofstream(FileName, std::ios::binary);
	iter = Nodes.begin();
	while (iter != Nodes.end())
	{
		TreeNode temp((Type)iter->playing, (unsigned short int*)iter->Board, (unsigned short int*)iter->parentBoard, iter->w, iter->n, iter->parentIndex);
		out.write((char*)&temp, sizeof(temp));
		iter++;
	}
	out.close();
}

void LoadWriteNodes::printHash()
{
	cout << Nodes.size() << endl;
	iter = Nodes.begin();
	while (iter != Nodes.end())
	{
		cout << iter->w << " " << iter->n << endl;
		iter++;
	}

}
