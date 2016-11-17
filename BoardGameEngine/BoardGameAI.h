#pragma once

#include "BoardGame.h"
#include "Trace.h"
#include "TTableReplaceByDepth.h"
#include "MovePicker.h"
#include <chrono>
#include <iostream>


//TODO: change to real functions
#define TRACE_IF(statement) do {if (options & AIOptions::DoTrace) {if (trace != nullptr) {statement;}}}while(0)
#define TRACE_POSITION_VALUE(value) TRACE_IF(trace->tracePositionValue(value))
#define TRACE_START_ALPHA_BETA_SEARCH(currentMaxDepth) TRACE_IF(trace->startAlphaBetaSearch(currentMaxDepth))
#define TRACE_FINISH_SEARCH() TRACE_IF(trace->finishSearch())
#define TRACE_MAKE_MOVE(move) TRACE_IF(trace->makeMove(move))
#define TRACE_UNDO_MOVE() TRACE_IF(trace->undoMove())
#define TRACE_START_SEARCH(game) TRACE_IF(trace->startSearch(game))
#define TRACE_ALPHA_BETA_WINDOW(alpha, beta) TRACE_IF(trace->alphaBetaWindow(alpha, beta))

namespace AIOptions
{
	enum {
		None =				1 << 0,
		DoTrace =			1 << 1, // trace steps during search
		Iterate =			1 << 2, // iterative deepening
		Evaluate =			1 << 3, // use evaluation function of game
		HistoryHeuristic =  1 << 4, // use history heuristics
		TimeManagement =	1 << 5 // cancel search after time is over
	};
}

//TODO: add positiion in book

//TODO: store result locally in BoardGameAI class?
class SearchResult
{
public:
	SearchResult(int16_t value, Move bestMove, int16_t depth) : value(value), bestMove(bestMove), depth(depth) {}
	int16_t value;
	Move bestMove;
	int16_t depth;
};

template<class GameType, bool enable>
class HistoryTableData
{
protected:
	int historyTable[2][BoardGame<GameType>::getHistoryStates()];
};

template<class GameType>
class HistoryTableData<GameType, false>
{

};

template<int tTableSizeMB>
class TTableData
{
protected:
	std::unique_ptr<TTableReplaceByDepth<tTableSizeMB>> tTable = std::make_unique<TTableReplaceByDepth<tTableSizeMB>>(); // transposition table
};

template<>
class TTableData<0>
{

};

template<bool enable>
class TimeManagementData
{
protected:
	std::chrono::time_point<std::chrono::system_clock> startTime;
	const int timeCheckInterval = 10000; // specifies after how many searched nodes the time is checked
	int nextTimeCheck; // incremented with every new node to search, time is checked if nextTimeCheck % timeCheckInterval == 0
	int maxTime; // maximum time to search in milli seconds
};

template<>
class TimeManagementData<false>
{

};


template<class GameType, int options, int tTableSizeMB>
class BoardGameAI :
	public HistoryTableData<GameType, HISTORY_ENABLED(options)>,
	public TTableData<tTableSizeMB>,
	public TimeManagementData<TIMER_ENABLED(options)>
{
private:
	std::shared_ptr<BoardGame<GameType>> game;
	std::vector<std::unique_ptr<MoveList>> movesCache; // to avoid recurring allocation of move arrays with each depth, they are cached here and reused
	std::shared_ptr<Trace<GameType>> trace = nullptr;
	Move bestRootMoveTmp; // best move found in the last iteration
	uint64_t nodes = 0; // number of nodes searched
	bool abortSearch; // set to true if time is over to abort the search
	bool depthReached; // flag shows if the last iteration reached the full depth, if not it's not necessary to search one level deeper
	static const int MOVE_NONE = BoardGame<GameType>::getNoMoveValue();

	void initializeSearch()
	{
		if constexpr(tTableSizeMB > 0)
		{
			tTable->new_search();
		}
		if constexpr(HISTORY_ENABLED(options))
		{
			memset(historyTable, 0, sizeof(historyTable));
		}
	}

	template<bool isRoot>
	int16_t alphaBeta(uint16_t depthLeft, int16_t alpha, int16_t beta)
	{
		if (abortSearch)
		{
			return 0;
		}

		if constexpr(TIMER_ENABLED(options))
		{
			if ((maxTime > 0) && (++nextTimeCheck % timeCheckInterval) == 0 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count() >= maxTime)
			{
				abortSearch = true;
				return 0;
			}
		}

		TRACE_ALPHA_BETA_WINDOW(alpha, beta);
		int16_t alphaOrig = alpha;

		uint64_t hash;
		Move bestMove = MOVE_NONE;

		nodes++;

		if constexpr(tTableSizeMB > 0)
		{
			hash = game->getHash();
			const TTEntry* ttEntry = tTable->probe(hash);
			if (ttEntry != nullptr)
			{
				if (ttEntry->depth() > depthLeft)
				{
					depthReached = true;
					if (ttEntry->bound() == BOUND_EXACT)
					{
						if constexpr(isRoot)
						{
							bestRootMoveTmp = ttEntry->move();
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
			depthReached = true;
			return evaluatePosition();
		}

		if (game->isGameOver())
		{
			return evaluatePosition();
		}

		MoveList *moves = movesCache.at(depthLeft).get();
		MovePicker<BoardGameAI<GameType, options, tTableSizeMB>, GameType, options> movePicker(*this, game, bestMove, moves);

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
				if constexpr(isRoot)
				{
					bestRootMoveTmp = move;
				}
				if (value > alpha)
				{
					alpha = value;
				}
				if constexpr(HISTORY_ENABLED(options))
				{
					// same algorithm as used by J. Tromp
					int moveIdx = 0;
					//tt move can be ignored here, because even if this was the best move it wouldn't get any extra points
					for (auto historyMoveIt = movePicker.sortedBegin(); historyMoveIt != movePicker.sortedEnd(); ++historyMoveIt)
					{
						Move historyMove = *historyMoveIt;
						moveIdx++;
						if (historyMove == bestMove)
						{
							historyTable[game->currentPlayer()][game->mapMoveToHistoryState(historyMove)] += moveIdx;
							break;
						}
						else
						{
							historyTable[game->currentPlayer()][game->mapMoveToHistoryState(historyMove)]--;
						}
					}
				}
			}
		}

		if constexpr(tTableSizeMB > 0)
		{
			tTable->store(hash, bestValue, bestValue <= alphaOrig ? BOUND_UPPER : (bestValue >= beta ? BOUND_LOWER : BOUND_EXACT), depthLeft, bestMove);
		}
		TRACE_POSITION_VALUE(bestValue);
		return bestValue;
	}

	template<int o = options, std::enable_if_t<ITERATE_ENABLED(o)>* = nullptr>
	SearchResult searchIterative(unsigned int maxDepth)
	{
		unsigned int currentMaxDepth = 0;
		int16_t score;
		int bestMove = MOVE_NONE;
		depthReached = true;
		initializeSearch();
		if constexpr(TIMER_ENABLED(o))
		{
			startTime = std::chrono::system_clock::now();
		}


		while (currentMaxDepth < maxDepth &&
			currentMaxDepth < MAX_PLY &&
			depthReached &&
			!abortSearch)
		{
			currentMaxDepth++;
			while (movesCache.size() <= currentMaxDepth) movesCache.push_back(std::make_unique<MoveList>());


			std::cout << "depth " << currentMaxDepth;
			TRACE_START_ALPHA_BETA_SEARCH(currentMaxDepth);
			depthReached = false;
			int scoreTmp = alphaBeta<true>(currentMaxDepth, -INFINITE, INFINITE);
			if (!abortSearch || currentMaxDepth == 0)
			{
				// only use best move/score from previous search if search was completed
				// otherwise best move/score is only calculated from the moves that were analyzed before abort
				bestMove = bestRootMoveTmp;
				score = scoreTmp;
			}
			//TODO: remove direct output and use an optional logger class 
			std::cout << " best move: " << bestMove << " score: " << score;
			std::cout << "\r";
			flush(std::cout);

		}
		TRACE_FINISH_SEARCH();
		return SearchResult(score, bestMove, currentMaxDepth);
	}

	template<int o = options, std::enable_if_t<!ITERATE_ENABLED(o)>* = nullptr>
	SearchResult searchIterative(unsigned int maxDepth)
	{
		initializeSearch();
		while (movesCache.size() <= maxDepth) movesCache.push_back(std::make_unique<MoveList>());

		TRACE_START_ALPHA_BETA_SEARCH(1);
		int16_t score = alphaBeta<true>(maxDepth, -INFINITE, INFINITE);
		TRACE_FINISH_SEARCH();
		return SearchResult(score, bestRootMoveTmp, maxDepth);
	}
	int16_t evaluatePosition()
	{
		if (game->hasWon())
		{
			int value = -(WINNING_VALUE - game->moveCounter);
			TRACE_POSITION_VALUE(value);
			return value;
		}
		if constexpr(options & AIOptions::Evaluate)
		{
			TRACE_POSITION_VALUE(game->evaluate());
			return game->evaluate();
		}
		TRACE_POSITION_VALUE(0);
		return 0;
	}

public:
	BoardGameAI() {}
	BoardGameAI(const BoardGameAI<GameType, options, tTableSizeMB>& other) {}
	~BoardGameAI() {}

	SearchResult search(const std::shared_ptr<BoardGame<GameType>>& gameArgument, unsigned int maxDepth = MAX_PLY, unsigned int maxTime = 0)
	{
		this->game = gameArgument;
		abortSearch = false;
		if constexpr(TIMER_ENABLED(options))
		{
			this->maxTime = maxTime;
		}
		TRACE_START_SEARCH(gameArgument);
		return searchIterative(maxDepth);

	}
	void setTrace(const std::shared_ptr<Trace<GameType>>& tracer)
	{
		this->trace = tracer;
	}
	uint64_t getAnalyzedPositions() const
	{
		return nodes;
	}
	friend class MovePicker<BoardGameAI<GameType, options, tTableSizeMB>, GameType, options>;
};





template<class GameType = void, int options = AIOptions::None, int tTableSizeMB = 0>
struct AIBuilder_t
{
	constexpr AIBuilder_t() {}

	// depth-first search is run repeatedly with increasing depth limits
	constexpr auto iterativeDeepening() const -> AIBuilder_t<GameType, options | AIOptions::Iterate, tTableSizeMB> { return{}; }
	// will save the game tree (only required for debugging)
	constexpr auto doTracing() const -> AIBuilder_t<GameType, options | AIOptions::DoTrace, tTableSizeMB> { return{}; }
	// if the node is not a final node then call the evaluation function of the game
	constexpr auto useEvaluationFunction() const -> AIBuilder_t<GameType, options | AIOptions::Evaluate, tTableSizeMB> { return{}; }
	// uses history heuristics
	constexpr auto useHistoryHeuristic() const -> AIBuilder_t<GameType, options | AIOptions::HistoryHeuristic, tTableSizeMB> { return{}; }
	// enable transposition tables, the default size is 128MB, it can be changed by calling useTTable<512>() to change it to 512MB
	template<int size = 128> constexpr auto useTTable() const -> AIBuilder_t<GameType, options, size> { return{}; }
	// enable time management (stops search after a fixed amount of time)
	constexpr auto enableTimeManagement() const -> AIBuilder_t<GameType, options | AIOptions::TimeManagement, tTableSizeMB> { return{}; }

	// use this method to create a BoardGameAI class with the specified options
	constexpr BoardGameAI<GameType, options, tTableSizeMB> create() const { return{}; }
};

template<class GameType>
using AIBuilder = AIBuilder_t<GameType>;
