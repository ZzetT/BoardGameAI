#include "stdafx.h"
#include "BoardGameAI.h"
#include <iostream>

template<bool DoTrace>
template<bool isRoot>
int BoardGameAI<DoTrace>::alphaBeta(unsigned int depthLeft, int alpha, int beta)
{
	if (abortSearch)
	{
		return 0;
	}

	if ((++nextTimeCheck % timeCheckInterval) == 0 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count() >= maxTime)
	{
		abortSearch = true;
		return 0;
	}

	TRACE_ALPHA_BETA_WINDOW(alpha, beta);
	if (depthLeft == 0) {
		TRACE_POSITION_VALUE(game->evaluate());
		return game->evaluate();
	}
	MoveList moves = movesCache.at(depthLeft);
	moves.clear();
	if (game->isGameOver() || (game->getMoves(&moves), moves.size()) == 0)
	{
		TRACE_POSITION_VALUE(game->evaluate());
		return game->evaluate();
	}
	int bestValue = -INFINITE;
	for (std::vector<int>::iterator moveIter = moves.begin(); moveIter != moves.end(); ++moveIter)
	{
		int move = *moveIter;
		game->makeMove(move);
		TRACE_MAKE_MOVE(move);
		int value = -alphaBeta<false>(depthLeft - 1, -beta, -alpha);
		game->undoMove();
		TRACE_UNDO_MOVE();

		if (abortSearch)
		{
			return 0;
		}


		if (value >= beta) {
			bestValue = value;
			break;
		}
		if (value > bestValue)
		{
			bestValue = value;
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
	TRACE_POSITION_VALUE(bestValue);
	return bestValue;
}

template<bool DoTrace> int BoardGameAI<DoTrace>::search(const std::shared_ptr<AbstractBoardGame>& gameArgument, unsigned int maxDepth, unsigned int _maxTime)
{
	unsigned int currentMaxDepth = 0;
	int score;
	int bestMove;
	this->game = gameArgument;
	this->maxTime = _maxTime;
	startTime = std::chrono::system_clock::now();
	abortSearch = false;
	TRACE_START_ITERATIVE_SEARCH(gameArgument);
	while (currentMaxDepth < maxDepth && !abortSearch)
	{
		currentMaxDepth++;
		while (movesCache.size() <= currentMaxDepth) {
			movesCache.push_back(MoveList());
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
		std::cout << " best move: " << bestMove << " score: " << score << std::endl;
	}
	TRACE_FINISH_SEARCH();
	return bestMove;
}


template class BoardGameAI<false>;
template class BoardGameAI<true>;