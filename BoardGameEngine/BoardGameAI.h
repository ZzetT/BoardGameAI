#pragma once

#include "AbstractBoardGame.h"
#include "Trace.h"
#include <chrono>

#define TRACE_IF(statement) do {if (DoTrace) {statement;}}while(0)

#define TRACE_POSITION_VALUE(value) TRACE_IF(trace->tracePositionValue(value))
#define TRACE_START_ALPHA_BETA_SEARCH(currentMaxDepth) TRACE_IF(trace->startAlphaBetaSearch(currentMaxDepth))
#define TRACE_FINISH_SEARCH() TRACE_IF(trace->finishSearch())
#define TRACE_MAKE_MOVE(move) TRACE_IF(trace->makeMove(move))
#define TRACE_UNDO_MOVE() TRACE_IF(trace->undoMove())
#define TRACE_START_ITERATIVE_SEARCH(game) TRACE_IF(trace->startIterativeSearch(game))
#define TRACE_ALPHA_BETA_WINDOW(alpha, beta) TRACE_IF(trace->alphaBetaWindow(alpha, beta))


template<bool DoTrace = false>
class BoardGameAI
{
private:
	std::shared_ptr<AbstractBoardGame> game;
	std::vector<std::unique_ptr<MoveList>> movesCache;
	std::shared_ptr<Trace> trace = nullptr;
	int bestMoveTmp;
	int nextTimeCheck;
	int maxTime;
	std::chrono::time_point<std::chrono::system_clock> startTime;
	const int timeCheckInterval = 10000;
	template<bool isRoot> int alphaBeta(unsigned int depth, int alpha, int beta);
	int evaluatePosition();
	bool abortSearch;

public:
	BoardGameAI() {};
	~BoardGameAI() {};

	static const int INFINITE = 200000;
	int search(const std::shared_ptr<AbstractBoardGame>& gameArgument, unsigned int maxDepth, unsigned int maxTime = 0);
	void setTrace(const std::shared_ptr<Trace>& tracer) { this->trace = tracer; }
};


