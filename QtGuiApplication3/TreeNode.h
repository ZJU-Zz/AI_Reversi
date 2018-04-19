#pragma once
#include <iostream>
#include <unordered_set>
enum Type
{
	EMPTY = 0,
	BLACK = 1,
	WHITE = 2
};

struct TreeNode
{
	Type playing;
	unsigned short int parentBoard[8];
	unsigned short int Board[8];
	unsigned int w;
	unsigned int n;
	int parentIndex;
	TreeNode(Type t,unsigned short int* B,unsigned short int* pB, unsigned int w, unsigned int n,int parentIndex)
	{
		for (int i = 0; i < 8; i++)
		{
			Board[i] = B[i];
			parentBoard[i] = pB[i];
		}
		playing = t;
		this->w = w;
		this->n = n;
		this->parentIndex = parentIndex;
	}

	TreeNode() 
	{
		w = 0;
		n = 0;
		parentIndex = 0;
	};
};


struct HashFunc
{
	std::size_t operator()(const TreeNode &key) const
	{
		using std::size_t;
		using std::hash;

		unsigned int res = 0;
		for (int i = 0; i < 8; i++)
		{
			res ^= key.Board[i];
			//res ^= key.parentBoard[i];
		}
		res ^= (int)key.playing;
		res ^= key.parentIndex;
		return size_t(res);
	}
};


struct EqualKey
{
	bool operator () (const TreeNode &lhs, const TreeNode &rhs) const
	{
		if (lhs.playing != rhs.playing || lhs.parentIndex != rhs.parentIndex) return false;
		for (int i = 0; i < 8; i++)
		{
			if (lhs.Board[i] != rhs.Board[i]) {
				return false;
			}
		}
		return true;
	}
};
