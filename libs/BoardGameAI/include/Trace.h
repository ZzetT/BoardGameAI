#pragma once

#include <cstdint>
#include "BoardGame.h"
#include <memory>

template<class GameType>
class Trace
{
public:
	Trace()
	{
	}
	virtual ~Trace()
	{

	}
	virtual void startSearch(const std::shared_ptr<BoardGame<GameType>>& /*game*/) {}
	virtual void tracePositionValue(int /*value*/) {  }
	virtual void startAlphaBetaSearch(unsigned int /*depth*/) {}
	virtual void alphaBetaWindow(int /*alpha*/, int /*beta*/) {}
	virtual void finishSearch() {}
	virtual void makeMove(int /*move*/) {}
	virtual void undoMove() {}
};

