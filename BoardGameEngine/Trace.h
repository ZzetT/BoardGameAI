#pragma once

#include <cstdint>
#include "AbstractBoardGame.h"
#include <memory>

class Trace
{
public:
	Trace();
	virtual ~Trace();
	virtual void startSearch(const std::shared_ptr<AbstractBoardGame>& /*game*/) {}
	virtual void tracePositionValue(int /*value*/) {  }
	virtual void startAlphaBetaSearch(unsigned int /*depth*/) {}
	virtual void alphaBetaWindow(int /*alpha*/, int /*beta*/) {}
	virtual void finishSearch() {}
	virtual void makeMove(int /*move*/) {}
	virtual void undoMove() {}
};

