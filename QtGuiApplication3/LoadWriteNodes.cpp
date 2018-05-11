#include "LoadWriteNodes.h"

LoadWriteNodes::LoadWriteNodes(unsigned short int * Board, Type playing,int count)
{	
	this->Board = Board;
	this->playing = playing;
	this->c_count = count;
}

LoadWriteNodes::LoadWriteNodes()
{
	need_store = true;
	ifstream in = ifstream(FileName, std::ios::binary);
	if (!in.is_open())	//如果文件不存在，则在树中插入一个初始节点，即刚开始的棋面
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
	if (!need_store) return;
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

void LoadWriteNodes::changeCurrent(unsigned short int * Board, Type playing, int count)
{
	this->Board = Board;
	this->playing = playing;
	this->c_count = count;
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


void LoadWriteNodes::run()
{
	TreeNode current(playing, Board , Board, 0, 0, 0);
	QTime reachTime = QTime::currentTime().addMSecs(1000 * 10);
	int count = 0;
	while (QTime::currentTime() < reachTime)
	{
		count++;
		Search(current);
	}
	cout << count << endl;
}


//Search部分与QtGuiApplicaton3.cpp中的search一致
void LoadWriteNodes::Search(TreeNode next)
{
	ReversiBoard TempBoard(next);
	double value[8][8] = { 0 };
	unsigned int playTime[8][8] = { 0 };
	unsigned int TotalPlayTime = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			vector<int> temp = TempBoard.canPlay(i, j);
			if (temp.size() > 0)
			{
				TreeNode Child = TempBoard.getLeaf(temp, i, j);
			Try:
				iter = Nodes.find(Child);
				if (iter != Nodes.end())
				{
					for (int t = 0; t < 8; t++)
					{
						if (iter->parentBoard[t] != TempBoard.getBoard()[t])
						{
							Child.parentIndex = iter->parentIndex + 1;
							goto Try;
						}
					}
					TotalPlayTime += iter->n;
					if (iter->n != 0) {
						value[i][j] = iter->w / (double)iter->n;
						playTime[i][j] = iter->n;
					}
				}
				else
				{
					Nodes.insert(Child);
					TempBoard.doPlay(temp, i, j);
					int tempcount = TempBoard.count;
					Type win = TempBoard.autoRandomPlay();
					backUp(Child, win, tempcount);
					return;
				}
			}
		}
	}

	Point bestChild = getBestChild(TotalPlayTime, value, playTime);

	if (bestChild.x != -1)
	{
		vector<int> playVector = TempBoard.canPlay(bestChild.x, bestChild.y);
		Search(TempBoard.getLeaf(playVector, bestChild.x, bestChild.y ));
	}
	else
	{
		if (TempBoard.isLeaf()) {
			int bcount = TempBoard.getCount(BLACK);
			int wcount = TempBoard.getCount(WHITE);
			if (bcount > wcount) {
				backUp(next, BLACK, TempBoard.count);
			}
			else if (bcount < wcount) {
				backUp(next, WHITE, TempBoard.count);
			}
			else
			{
				backUp(next, EMPTY, TempBoard.count);
			}
			return;
		}
		TempBoard.reversePlaying();
		TreeNode pass(TempBoard.getPlaying(), TempBoard.getBoard(), TempBoard.getBoard(), 0, 0, 0);
	Try1:
		iter = Nodes.find(pass);
		if (iter != Nodes.end())
		{
			for (int t = 0; t < 8; t++)
			{
				if (iter->parentBoard[t] != TempBoard.getBoard()[t])
				{
					pass.parentIndex = iter->parentIndex + 1;
					goto Try1;
				}
			}
		}
		else
		{
			Nodes.insert(pass);
		}
		Search(pass);
	}
}

void LoadWriteNodes::backUp(TreeNode next, Type win, int count)
{
	for (int i = 0;; i++)
	{
		TreeNode temp = TreeNode(next.playing, next.Board, next.Board, 0, 0, i);
		iter = Nodes.find(temp);
		if (iter != Nodes.end())
		{
			TreeNode newNode = TreeNode(next.playing, next.Board, (unsigned short int*)iter->parentBoard, iter->w, iter->n + 1, i);
			if (win == next.playing)
			{
				newNode.w++;
			}
			Nodes.erase(temp);
			Nodes.insert(newNode);
			if (newNode.parentBoard[0] == 0xFFFF) return;	//代表这是全树的root节点
			Type pType = (next.playing == BLACK) ? WHITE : BLACK;
			TreeNode parentNode = TreeNode(pType, newNode.parentBoard, newNode.parentBoard, 0, 0, 0);
			int flag = 0;
			for (int t = 0; t < 8; t++)
			{
				if (newNode.parentBoard[t] != newNode.Board[t]) {
					if (count - 1 >= c_count) backUp(parentNode, win, count - 1);//为了提高速度，回溯深度限制，不需要回溯到最上面的root节点，只需要回溯到当前局面深度即可
					flag = 1;
					break;
				}
			}
			if (flag == 0 && count >= c_count) backUp(parentNode, win, count);
		}
		else
		{
			break;
		}
	}
}

Point LoadWriteNodes::getBestChild(unsigned int total, double value[][8], unsigned int playTime[][8])
{
	Point ret;
	ret.x = ret.y = -1;
	double bestValue = 0;
	double ln = log(total);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (playTime[i][j] > 0)
			{
				double newValue = (1 - value[i][j]) + C * sqrt(ln / playTime[i][j]);
				if (newValue >= bestValue)
				{
					bestValue = newValue;
					ret.x = i;
					ret.y = j;
				}
			}
		}
	}
	return ret;
}

