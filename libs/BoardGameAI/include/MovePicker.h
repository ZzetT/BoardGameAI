#pragma once

#include <memory>
#include <vector>
#include <cassert>
#include "BoardGame.h"
#include "BoardGameAI.h"
#include "SmartVector.h"

enum State
{
	TTMOVE, GENERATE_MOVES, MOVELIST
};

template<class AIType, class GameType, int options>
class MovePicker
{
	typedef typename GameType::MovesPerPositionType MovesPerPositionType;

public:
	MovePicker(const AIType& ai, const std::shared_ptr<GameType>& game, Move ttMove, MovesPerPositionType *moves)
		: game(game), ai(ai), ttMove(ttMove), moves(moves)
	{
		state = (ttMove == MOVE_NONE ? GENERATE_MOVES : TTMOVE);
	}

	~MovePicker() {};
	typename MovesPerPositionType::const_iterator sortedBegin()
	{
		return moves->data()->begin();
	}
	typename MovesPerPositionType::const_iterator sortedEnd()
	{
		if (state == TTMOVE || state == GENERATE_MOVES)
		{
			return moves->data()->begin();
		}
		return moveIter;
	}
	Move nextMove()
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
			STATIC_IF(HISTORY_ENABLED(options))
			{
				for (auto it = moves->data()->begin(); it != moves->data()->end(); ++it)
				{
					it->value = ai.historyTable[game->currentPlayer()][game->mapMoveToHistoryState(*it)];
				}
			}
			moveIter = moves->data()->begin();
			moveEndIter = moves->data()->end();

			state = MOVELIST;
		case MOVELIST:
			while (moveIter != moveEndIter)
			{
				move = pick_best(moveIter++, moveEndIter);
				assert(move != MOVE_NONE);
				if (move != ttMove)
				{
					return move;
				}
			}
			break;
		}
		return MOVE_NONE;
	}

private:
	const std::shared_ptr<GameType> game;
	const AIType& ai;
	Move ttMove;
	State state;
	MovesPerPositionType* moves;
	static const int MOVE_NONE = GameType::getNoMoveValue();

	typename MovesPerPositionType::iterator moveIter;
	typename MovesPerPositionType::iterator moveEndIter;

	Move pick_best(typename MovesPerPositionType::iterator begin, typename MovesPerPositionType::iterator end)
	{
		std::swap(*begin, *std::max_element(begin, end, [](ExtMove a, ExtMove b) { return a.value < b.value; }));
		return begin->move;
	}
};

