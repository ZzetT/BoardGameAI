#pragma once

#include "BoardGame.h"
#include "TTableReplaceByDepth.h"
#include "MovePicker.h"
#include "StatusUpdate.h"
#include <chrono>
#include <cstring>


class SearchResult
{
public:
	SearchResult(int16_t value, Move bestMove, int16_t depth) : value(value), bestMove(bestMove), depth(depth) {}
	int16_t value;
	Move bestMove;
	int16_t depth;
};

template<class GameType, int options, int tTableSizeMB>
class BoardGameAI 
{
private:
	std::shared_ptr<GameType> game;
	Move bestRootMoveTmp; // best move found in the last iteration
	uint64_t nodes = 0; // number of nodes searched
	bool abortSearch; // set to true if time is over to abort the search
	bool depthReached; // flag shows if the last iteration reached the full depth, if not it's not necessary to search one level deeper
	static const int MOVE_NONE = GameType::noMoveValue();
	static const int MAX_PLY = GameType::MAX_PLY;

	//time management data
	std::chrono::time_point<std::chrono::system_clock> startTime;
	const int timeCheckInterval = 10000; // specifies after how many searched nodes the time is checked
	int nextTimeCheck; // incremented with every new node to search, time is checked if nextTimeCheck % timeCheckInterval == 0
	int maxTime; // maximum time to search in milli seconds

	//transposition table
	std::unique_ptr<TTableReplaceByDepth<tTableSizeMB>> tTable = std::make_unique<TTableReplaceByDepth<tTableSizeMB>>();


	//update stats during search
	StatusUpdate *statsUpdate;

	//history table
	unsigned int counterMove[GameType::counterMoveStates()];

	void initializeSearch()
	{
		//if constexpr(tTableSizeMB > 0)
		STATIC_IF(tTableSizeMB > 0)
		{
			this->tTable->new_search();
		}
		STATIC_IF(COUNTERMOVE_ENABLED(options))
		{
			memset(this->counterMove, MOVE_NONE, sizeof(this->counterMove));
		}
	}

	template<bool isRoot>
	int16_t alphaBeta(uint16_t depthLeft, int16_t alpha, int16_t beta)
	{
		if (abortSearch)
		{
			return 0;
		}

		STATIC_IF(TIMER_ENABLED(options))
		{
			if ((this->maxTime > 0) && (++this->nextTimeCheck % this->timeCheckInterval) == 0 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->startTime).count() >= this->maxTime)
			{
				abortSearch = true;
				return 0;
			}
		}

		int16_t alphaOrig = alpha;
		bool searchPv = true;
		uint64_t hash;
		Move bestMove = MOVE_NONE;

		STATIC_IF(tTableSizeMB > 0)
		{
			hash = game->hash();
			const TTEntry* ttEntry = this->tTable->probe(hash);
			if (ttEntry != nullptr)
			{
				if (ttEntry->depth() > depthLeft)
				{
					depthReached = true;
					if (ttEntry->bound() == BOUND_EXACT)
					{
						STATIC_IF(isRoot)
						{
							bestRootMoveTmp = ttEntry->move();
						}
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

		STATIC_IF(MATE_DISTANCE_PRUNING_ENABLED(options))
		{
			int16_t maxBeta = game->maxPossibleEvaluation();
			if (beta > maxBeta)
			{
				beta = maxBeta;
				if (alpha >= beta)
				{
					return beta;
				}
			}
		}

		int16_t bestValue = -INFINITE;
		Move move;
		MovePicker<BoardGameAI<GameType, options, tTableSizeMB>> movePicker(*this, game.get(), bestMove);
		while ((move = movePicker.nextMove()) != MOVE_NONE)
		{
			game->makeMove(move);
			nodes++;
			int16_t value;

			STATIC_IF(PVS_ENABLED(options))
			{
				if (searchPv)
				{
					value = -alphaBeta<false>(depthLeft - 1, -beta, -alpha);
				}
				else
				{
					value = -alphaBeta<false>(depthLeft - 1, -alpha - 1, -alpha);
					if (value > alpha && value < beta) //fail-soft condition
					{
						value = -alphaBeta<false>(depthLeft - 1, -beta, -alpha); // re-search
					}
				}
			}
			else
			{
				value = -alphaBeta<false>(depthLeft - 1, -beta, -alpha);
			}
			game->undoMove();
			

			if (abortSearch)
			{
				return 0;
			}

			if (value >= beta) {
				// beta cut
				bestValue = value;
				bestMove = move;
				STATIC_IF(isRoot)
				{
					bestRootMoveTmp = move;
				}
				STATIC_IF(COUNTERMOVE_ENABLED(options) && !isRoot)
				{
					counterMove[game->mapLastMoveToCounterMoveState()] = move;
				}
				break;
			}
			if (value > bestValue)
			{
				bestValue = value;
				bestMove = move;
				STATIC_IF(isRoot)
				{
					bestRootMoveTmp = move;
				}
				if (value > alpha)
				{
					alpha = value;
					STATIC_IF(PVS_ENABLED(options))
					{
						searchPv = false;
					}
				}
			}
		}
		STATIC_IF(tTableSizeMB > 0)
		{
			this->tTable->store(hash, bestValue, bestValue <= alphaOrig ? BOUND_UPPER : (bestValue >= beta ? BOUND_LOWER : BOUND_EXACT), depthLeft, bestMove);
		}
		return bestValue;
	}

	template<int o = options, std::enable_if_t<ITERATE_ENABLED(o)>* = nullptr>
	SearchResult searchIterative(unsigned int maxDepth)
	{
		unsigned int currentMaxDepth = 0;
		int16_t score = 0;
		int bestMove = MOVE_NONE;
		depthReached = true;
		initializeSearch();
		STATIC_IF(TIMER_ENABLED(o))
		{
			this->startTime = std::chrono::system_clock::now();
		}


		while (currentMaxDepth < maxDepth &&
			currentMaxDepth < MAX_PLY &&
			depthReached &&
			!abortSearch)
		{
			currentMaxDepth++;

			if (UPDATE_STATUS_ENABLED(o))
			{
				statsUpdate->sendCurrentDepth(currentMaxDepth);
			}

			depthReached = false;
			int scoreTmp = alphaBeta<true>(currentMaxDepth, -INFINITE, INFINITE);
			if (!abortSearch)
			{
				// only use best move/score from previous search if search was completed
				// otherwise best move/score is only calculated from the moves that were analyzed before abort
				bestMove = bestRootMoveTmp;
				score = scoreTmp;
			}
			if (UPDATE_STATUS_ENABLED(o))
			{
				statsUpdate->sendPrelimaryResult(currentMaxDepth, bestMove, score);
			}
		}
		return SearchResult(score, bestMove, currentMaxDepth);
	}

	template<int o = options, std::enable_if_t<!ITERATE_ENABLED(o)>* = nullptr>
	SearchResult searchIterative(unsigned int maxDepth)
	{
		initializeSearch();

		int16_t score = alphaBeta<true>(maxDepth, -INFINITE, INFINITE);
		return SearchResult(score, bestRootMoveTmp, maxDepth);
	}
	int16_t evaluatePosition()
	{
		return game->evaluate();
	}

public:
	typedef typename GameType AIGameType;
	static const int AIOptions = options;

	template<int o = options, std::enable_if_t<UPDATE_STATUS_ENABLED(o)>* = nullptr>
	BoardGameAI(StatusUpdate *statusUpdate) : statsUpdate(statusUpdate) {}

	template<int o = options, std::enable_if_t<!UPDATE_STATUS_ENABLED(o)>* = nullptr>
	BoardGameAI() {}


	BoardGameAI(const BoardGameAI<GameType, options, tTableSizeMB>& other) { (void)other; }
	~BoardGameAI() {}

	SearchResult search(const std::shared_ptr<GameType>& gameArgument, unsigned int maxDepth = MAX_PLY, unsigned int maxTime = 0)
	{
		this->game = gameArgument;
		abortSearch = false;
		STATIC_IF(TIMER_ENABLED(options))
		{
			this->maxTime = maxTime;
		}
		return searchIterative(maxDepth);

	}
	uint64_t analyzedPositions() const
	{
		return nodes;
	}
	void resetAnalyzedPositionsCounter()
	{
		nodes = 0;
	}
	friend class MovePicker<BoardGameAI<GameType, options, tTableSizeMB>>;
	friend class MovePickerHandling<BoardGameAI<GameType, options, tTableSizeMB>, true>;
};





template<class GameType = void, int options = AIOptions::None, int tTableSizeMB = 0>
struct AIBuilder_t
{
	constexpr AIBuilder_t() {}

	// depth-first search is run repeatedly with increasing depth limits
	constexpr auto iterativeDeepening() const -> AIBuilder_t<GameType, options | AIOptions::Iterate, tTableSizeMB> { return{}; }
	// use prinicipal variation search
	constexpr auto usePVS() const -> AIBuilder_t<GameType, options | AIOptions::PVS, tTableSizeMB> { return{}; }
	// uses counter move heuristics
	constexpr auto useCouterMoveHeuristic() const -> AIBuilder_t<GameType, options | AIOptions::CounterMove, tTableSizeMB> { return{}; }
	// uses mate distance pruning
	constexpr auto useMateDistancePruning() const -> AIBuilder_t<GameType, options | AIOptions::MateDistancePruning, tTableSizeMB> { return{}; }
	// enable transposition tables, the default size is 128MB, it can be changed by calling useTTable<512>() to change it to 512MB
	template<int size = 128> constexpr auto useTTable() const -> AIBuilder_t<GameType, options, size> { return{}; }
	// enable time management (stops search after a fixed amount of time)
	constexpr auto enableTimeManagement() const -> AIBuilder_t<GameType, options | AIOptions::TimeManagement, tTableSizeMB> { return{}; }
	// update statistics during search e.g. current depth, position value, analyzed nodes etc.
	constexpr auto updateStats() const -> AIBuilder_t<GameType, options | AIOptions::UpdateStatus, tTableSizeMB> { return{}; }

	// use this method to create a BoardGameAI class with the specified options
	constexpr BoardGameAI<GameType, options, tTableSizeMB> create() const { return{}; }

	constexpr BoardGameAI<GameType, options, tTableSizeMB> create(StatusUpdate *statusUpdate) const { return{ statusUpdate }; }
};

template<class GameType>
using AIBuilder = AIBuilder_t<GameType>;
