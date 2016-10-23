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
	moveHistory.undoMove();
	moveCounter--;
}

void AbstractBoardGame::makeMoves(std::vector<Move>& moves)
{
	for (auto move : moves)
	{
		makeMove(move);
	}
}

void AbstractBoardGame::makeMove(Move n)
{
	handleMakeMove(n);
	moveHistory.addMove(n);
	moveCounter++;
}
