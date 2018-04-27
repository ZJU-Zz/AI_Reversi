#include "ReversiBoard.h"

bool ReversiBoard::setType(Type t, int x, int y)
{
	Board[x] &= (~(0x3 << ((7 - y) * 2)));
	Board[x] |= (int)t << ((7 - y) * 2);
	return true;
}

vector<int> ReversiBoard::canPlay( int x, int y)
{
	vector<int> a;
	if (getType(x, y) != EMPTY) return a;
	for (int i = 0; i < 8; i++)
	{
		int flag = 0;
		int tempx = x + direction[i][0];
		int tempy = y + direction[i][1];
		while (isValid(tempx, tempy) && getType(tempx,tempy) != EMPTY)
		{
			if (getType(tempx, tempy) == playing) 
			{
				if (flag) 
				{
					a.push_back(i);
					break;
				}
				else
				{
					break;
				}
			}
			else
			{
				flag = 1;
			}
			tempx += direction[i][0];
			tempy += direction[i][1];
		}
	}
	return a;
}

void ReversiBoard::doPlay(vector<int> dir, int x, int y)
{
	int size = dir.size();
	if (size == 0) return;
	for (int i = 0; i < 8; i++)
	{
		lastBoard[i] = Board[i];
	}
	setType(playing, x, y);
	for (int i = 0; i < size; i++)
	{
		int d = dir[i];
		int tempx = x + direction[d][0];
		int tempy = y + direction[d][1];
		while (isValid(tempx, tempy) && getType(tempx, tempy) != EMPTY)
		{
			if (getType(tempx, tempy) == playing)
			{
				break;
			}
			else
			{
				setType(playing, tempx, tempy);
				tempx += direction[d][0];
				tempy += direction[d][1];
			}
		}
	}
	reversePlaying();
	canRegret = true;
	count++;
}

bool ReversiBoard::isValid(int x, int y)
{
	return (x >= 0 && x <= 7) && (y >= 0 && y <= 7);
}

void ReversiBoard::printBoard()
{
	for (int i = 0; i < 8; i++)
	{
		unsigned short int num = Board[i];
		string temp = "";
		int j = 16;
		while (j--)
		{
			temp = to_string(num % 2) + temp;
			num /= 2;
			if (j % 2 == 0) temp = " " + temp;
		}
		cout << temp << endl;
	}
}


void ReversiBoard::reset()
{
	for (int i = 0; i < 8; i++) {
		Board[i] = 0;
	}
	setType(WHITE, 3, 3);
	setType(WHITE, 4, 4);
	setType(BLACK, 3, 4);
	setType(BLACK, 4, 3);
	playing = BLACK;
	end = 0;

	for (int i = 0; i < 8; i++)
	{
		lastBoard[i] = Board[i];
	}
	count = 4;
}

Type ReversiBoard::getPlaying()
{
	return playing;
}

void ReversiBoard::reversePlaying()
{
	playing = (playing == BLACK) ? WHITE : BLACK;
}

void ReversiBoard::takeBack()
{
	if (canRegret) {
		reversePlaying();
		for (int i = 0; i < 8; i++)
		{
			Board[i] = lastBoard[i];
		}
		canRegret = false;
	}
}

int ReversiBoard::getCount(Type p)
{
	int count = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (getType(i, j) == p) count++;
		}
	}
	return count;
}

void ReversiBoard::autoRandomPlayOneStep()
{
	for (int i = 0; i < 60; i++)
	{
		int m = smartDirection[i][0];
		int n = smartDirection[i][1];
		cout << "(" << m << "," << n <<  ")" << endl;
		vector<int> temp = canPlay(m, n);
		if (temp.size() != 0)
		{
			doPlay(temp, m, n);
			end = 0;
			return;
		}
	}
	end++;
	if (end == 2)
	{
		return;
	}
	else
	{
		reversePlaying();
		return;
	}
}

Type ReversiBoard::autoRandomPlay()
{
	for (int i = 0; i < 60; i++)
	{
		
		int m = smartDirection[i][0];
		int n = smartDirection[i][1];
		//templist.erase(templist.begin() + x);
		vector<int> temp = canPlay(m, n);
		if (temp.size() != 0)
		{
			doPlay(temp, m, n);
			end = 0;
			return autoRandomPlay();
		}
	}
	end++;
	if (end == 2)
	{
		int bcount = getCount(BLACK);
		int wcount = getCount(WHITE);
		if (bcount > wcount)
		{
			return BLACK;
		}
		else if (bcount < wcount)
		{
			return WHITE;
		}
		else
		{
			return EMPTY;
		}
	}
	else
	{
		reversePlaying();
		return autoRandomPlay();
	}
}

bool ReversiBoard::isLeaf()
{
	ReversiBoard temp = *this;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (temp.canPlay(i, j).size() > 0) {
				return false;
			}
		}
	}
	temp.reversePlaying();
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (temp.canPlay(i, j).size() > 0) {
				return false;
			}
		}
	}
	return true;
}

TreeNode ReversiBoard::getLeaf(vector<int> dir, int x, int y)
{
	ReversiBoard temp = *this;
	temp.doPlay(dir, x, y);
	/*
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			cout << (temp.getBoard()[i] == this->getBoard()[i]) << " ";
		}
		cout << endl;
	}
	system("pause");
	*/
	return TreeNode(temp.getPlaying(), temp.getBoard(),this->getBoard(), 0, 0,0);
	
}

bool ReversiBoard::isEnd()
{
	if (end == 2) return true;
	return false;
}

unsigned short int* ReversiBoard::getBoard()
{
	return Board;
}

Type ReversiBoard::getType(int x, int y)
{
	int temp = Board[x] >> ((7 - y) * 2) & 0x3;
	return (Type)temp;
}

ReversiBoard::ReversiBoard()
{

}

ReversiBoard::ReversiBoard(TreeNode n)
{
	for (int i = 0; i < 8; i++)
	{
		Board[i] = n.Board[i];
	}
	playing = n.playing;
	end = 0;
	count = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (getType(i, j) != EMPTY) count++;
		}
	}
}



ReversiBoard::~ReversiBoard()
{
}
