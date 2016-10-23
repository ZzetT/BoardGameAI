#pragma once

#include <memory>
#include <vector>
#include <cassert>
#include "AbstractBoardGame.h"

enum State
{
	TTMOVE, GENERATE_MOVES, MOVELIST
};

template<int MOVE_NONE>
class MovePicker
{
public:
	MovePicker(const std::shared_ptr<AbstractBoardGame>& game, Move ttMove, MoveList *moves)
		: game(game), ttMove(ttMove), moves(moves)
	{
		state = (ttMove == MOVE_NONE ? GENERATE_MOVES : TTMOVE);
	}

	~MovePicker() {};
	
	Move nextMove();
private:
	std::shared_ptr<AbstractBoardGame> game;
	Move pick_best(std::vector<ExtMove>::iterator begin, std::vector<ExtMove>::iterator end);
	Move ttMove;
	State state;
	MoveList *moves;
	std::vector<ExtMove>::iterator moveIter;
};

template<int MOVE_NONE>
Move MovePicker<MOVE_NONE>::pick_best(std::vector<ExtMove>::iterator begin, std::vector<ExtMove>::iterator end)
{
	std::swap(*begin, *std::max_element(begin, end, [](ExtMove a, ExtMove b) { return a.value < b.value; }));
	return begin->move;
}

template<int MOVE_NONE>
Move MovePicker<MOVE_NONE>::nextMove()
{
	Move move;
	switch (state)
	{
	case TTMOVE:
		state = GENERATE_MOVES;
		return ttMove;
	case GENERATE_MOVES:
		moves->clear();
		game->getMoves(moves);
		moveIter = moves->data()->begin();
		state = MOVELIST;
	case MOVELIST:
		while (moveIter != moves->data()->end())
		{
			move = pick_best(moveIter++, moves->data()->end());
			assert(move != MOVE_NONE);
			if (move != ttMove)
			{
				return move;
			}
		}
	}
	return MOVE_NONE;
}
