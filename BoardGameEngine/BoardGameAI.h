#pragma once

#include "AbstractBoardGame.h"
#include "Trace.h"
#include "TTableReplaceByDepth.h"
#include "MovePicker.h"
#include <chrono>
#include <iostream>

#define TRACE_IF(statement) do {if (options & AIOptions::DoTrace) {if (trace != nullptr) {statement;}}}while(0)
#define TRACE_POSITION_VALUE(value) TRACE_IF(trace->tracePositionValue(value))
#define TRACE_START_ALPHA_BETA_SEARCH(currentMaxDepth) TRACE_IF(trace->startAlphaBetaSearch(currentMaxDepth))
#define TRACE_FINISH_SEARCH() TRACE_IF(trace->finishSearch())
#define TRACE_MAKE_MOVE(move) TRACE_IF(trace->makeMove(move))
#define TRACE_UNDO_MOVE() TRACE_IF(trace->undoMove())
#define TRACE_START_SEARCH(game) TRACE_IF(trace->startSearch(game))
#define TRACE_ALPHA_BETA_WINDOW(alpha, beta) TRACE_IF(trace->alphaBetaWindow(alpha, beta))

class SearchResult
{
public:
	SearchResult(int16_t value, Move bestMove, int16_t depth) : value(value), bestMove(bestMove), depth(depth) {}
	int16_t value;
	Move bestMove;
	int16_t depth;
};

namespace AIOptions
{
	enum { None = 0, DoTrace = 1, Iterate = 2 };
}

template<int options, int tTableSizeMB, int MOVE_NONE >
class BoardGameAI
{
private:
	std::shared_ptr<AbstractBoardGame> game;
	std::vector<std::unique_ptr<MoveList>> movesCache;
	std::shared_ptr<Trace> trace = nullptr;
	Move bestMoveTmp;
	int nextTimeCheck;
	int maxTime;
	uint64_t nodes = 0;
	std::chrono::time_point<std::chrono::system_clock> startTime;
	const int timeCheckInterval = 10000;
	bool abortSearch;
	std::unique_ptr<TTableReplaceByDepth<tTableSizeMB>> tTable = std::make_unique<TTableReplaceByDepth<tTableSizeMB>>();

	template<bool isRoot> int16_t alphaBeta(uint16_t depth, int16_t alpha, int16_t beta);
	int16_t evaluatePosition();

public:
	BoardGameAI() {}
	BoardGameAI(const BoardGameAI<options, tTableSizeMB, MOVE_NONE>& other) {}
	~BoardGameAI() {}

	SearchResult search(const std::shared_ptr<AbstractBoardGame>& gameArgument, unsigned int maxDepth = MAX_PLY, unsigned int maxTime = 0);
	void setTrace(const std::shared_ptr<Trace>& tracer) { this->trace = tracer; }
	uint64_t getAnalyzedPositions() const;
};

template<int options, int tTableSizeMB, int MOVE_NONE>
template<bool isRoot>
int16_t BoardGameAI<options, tTableSizeMB, MOVE_NONE>::alphaBeta(uint16_t depthLeft, int16_t alpha, int16_t beta)
{
	if (abortSearch)
	{
		return 0;
	}

	if ((maxTime > 0) && (++nextTimeCheck % timeCheckInterval) == 0 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count() >= maxTime)
	{
		abortSearch = true;
		return 0;
	}

	TRACE_ALPHA_BETA_WINDOW(alpha, beta);
	int16_t alphaOrig = alpha;

	uint64_t hash;
	Move bestMove = MOVE_NONE; //NONE

	nodes++;

	if (tTableSizeMB != 0)
	{
		hash = game->getHash();
		const TTEntry* ttEntry = tTable->probe(hash);
		if (ttEntry != nullptr)
		{
			if (ttEntry->depth() > depthLeft)
			{
				if (ttEntry->bound() == BOUND_EXACT)
				{
					if (isRoot)
					{
						bestMoveTmp = ttEntry->move();
					}
					TRACE_POSITION_VALUE(ttEntry->value());
					return ttEntry->value();
				}
				else if (ttEntry->bound() == BOUND_LOWER)
				{
					alpha = std::max(alpha, ttEntry->value());
				}
				else if (ttEntry->bound() == BOUND_UPPER)
				{
					beta = std::min(beta, ttEntry->value());
				}
				if (alpha >= beta)
				{
					TRACE_POSITION_VALUE(ttEntry->value());
					return ttEntry->value();
				}
			}
			bestMove = ttEntry->move();
		}
	}
	if (depthLeft == 0) {
		return evaluatePosition();
	}

	if (game->isGameOver())
	{
		return evaluatePosition();
	}

	MoveList *moves = movesCache.at(depthLeft).get();
	MovePicker<MOVE_NONE> movePicker(game, bestMove, moves);

	int16_t bestValue = -INFINITE;
	Move move;
	while ((move = movePicker.nextMove()) != MOVE_NONE)
	{
		game->makeMove(move);
		TRACE_MAKE_MOVE(move);
		int16_t value = -alphaBeta<false>(depthLeft - 1, -beta, -alpha);
		game->undoMove();
		TRACE_UNDO_MOVE();

		if (abortSearch)
		{
			return 0;
		}

		if (value >= beta) {
			bestValue = value;
			bestMove = move;
			break;
		}
		if (value > bestValue)
		{
			bestValue = value;
			bestMove = move;
			if (isRoot)
			{
				bestMoveTmp = move;
			}
			if (value > alpha)
			{
				alpha = value;
			}
		}
	}

	if (tTableSizeMB != 0)
	{
		tTable->store(hash, bestValue, bestValue <= alphaOrig ? BOUND_UPPER : (bestValue >= beta ? BOUND_LOWER : BOUND_EXACT), depthLeft, bestMove);
	}
	TRACE_POSITION_VALUE(bestValue);
	return bestValue;
}

template<int options, int tTableSizeMB, int MOVE_NONE>
int16_t BoardGameAI<options, tTableSizeMB, MOVE_NONE>::evaluatePosition()
{
	if (game->hasWon())
	{
		int value = -(WINNING_VALUE - game->moveCounter);
		TRACE_POSITION_VALUE(value);
		return value;
	}
	TRACE_POSITION_VALUE(game->evaluate());
	return game->evaluate();
}

template<int options, int tTableSizeMB, int MOVE_NONE>
SearchResult BoardGameAI<options, tTableSizeMB, MOVE_NONE>::search(const std::shared_ptr<AbstractBoardGame>& gameArgument, unsigned int maxDepth, unsigned int _maxTime)
{
	unsigned int currentMaxDepth = 0;
	int16_t score;
	int bestMove = MOVE_NONE;
	this->game = gameArgument;
	abortSearch = false;
	TRACE_START_SEARCH(gameArgument);
	if (options & AIOptions::Iterate)
	{
		this->maxTime = _maxTime;
		if (tTableSizeMB != 0)
		{
			tTable->new_search();
		}
		startTime = std::chrono::system_clock::now();
		while (currentMaxDepth < maxDepth && currentMaxDepth < MAX_PLY && !abortSearch)
		{
			currentMaxDepth++;
			while (movesCache.size() <= currentMaxDepth) {
				movesCache.push_back(std::make_unique<MoveList>());
			}

			std::cout << "depth " << currentMaxDepth;
			TRACE_START_ALPHA_BETA_SEARCH(currentMaxDepth);
			int scoreTmp = alphaBeta<true>(currentMaxDepth, -INFINITE, INFINITE);
			if (!abortSearch || currentMaxDepth == 0)
			{
				// only use best move/score from previous search if search was completed
				// otherwise best move/score is only calculated from the moves that were analyzed before abort
				bestMove = bestMoveTmp;
				score = scoreTmp;
			}
			std::cout << " best move: " << bestMove << " score: " << score;
			std::cout << "\r";
			flush(std::cout);

		}
	}
	else
	{
		while (movesCache.size() <= maxDepth) {
			movesCache.push_back(std::make_unique<MoveList>());
		}
		TRACE_START_ALPHA_BETA_SEARCH(1);
		score = alphaBeta<true>(maxDepth, -INFINITE, INFINITE);
		bestMove = bestMoveTmp;
	}
	TRACE_FINISH_SEARCH();
	return SearchResult(score, bestMove, currentMaxDepth);
}

template<int options, int tTableSizeMB, int MOVE_NONE>
uint64_t BoardGameAI<options, tTableSizeMB, MOVE_NONE>::getAnalyzedPositions() const
{
	return nodes;
}


template<int options = AIOptions::None, int tTableSizeMB = 0, int MOVE_NONE = 0>
struct AIBuilder_t
{
	constexpr AIBuilder_t() {}

	constexpr auto iterativeDeepening() const -> AIBuilder_t<options | AIOptions::Iterate, tTableSizeMB, MOVE_NONE> { return{}; }
	constexpr auto doTracing() const -> AIBuilder_t<options | AIOptions::DoTrace, tTableSizeMB, MOVE_NONE> { return{}; }
	template<int none> constexpr auto setMoveNone() const -> AIBuilder_t<options, tTableSizeMB, none> { return{}; }
	template<int size = 128> constexpr auto useTTable() const -> AIBuilder_t<options, size, MOVE_NONE> { return{}; }


	constexpr BoardGameAI<options, tTableSizeMB, MOVE_NONE> create() const { return{}; }
};
using AIBuilder = AIBuilder_t<>;
