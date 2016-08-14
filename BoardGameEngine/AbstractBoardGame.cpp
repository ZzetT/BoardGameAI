#include "stdafx.h"
#include "AbstractBoardGame.h"


AbstractBoardGame::AbstractBoardGame()
{
}


AbstractBoardGame::~AbstractBoardGame()
{
}

void AbstractBoardGame::undoMove()
{
	handleUndoMove();
	moveHistory.pop_back();
	moveCounter--;
}

void AbstractBoardGame::makeMoves(std::vector<int>& moves)
{
	for (auto move : moves)
	{
		makeMove(move);
	}
}

void AbstractBoardGame::makeMove(int n)
{
	handleMakeMove(n);
	moveHistory.push_back(n);
	moveCounter++;
}
