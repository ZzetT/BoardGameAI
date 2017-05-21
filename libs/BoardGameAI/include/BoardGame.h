#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <cassert>
#include "MoveList.h"
#include "types.h"

/*
 * The design for this class is based on the "Curiously recurring template pattern"
 * to save the runtime overhead of virtual functions.
 * See https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern for more details
 *
 * @param maxPly can be set if the upper bound of plies for a whole game is known (this avoids dynamic reallocation and increases the performance), 0 will use dynamic allocation
 * @param maxMoves can be set if the upper bound of moves for any position is known (this avoids dynamic reallocation and increases the performance), 0 will use dynamic allocation,
 *        this value has only an effect if useStaticMoveEvaluation is enabled
 */

template <typename DerivedBoardGame, bool useStaticMoveEvaluation = false, int maxPly = 0, int maxMoves = 0>
class BoardGame
{
public:
	typedef typename std::conditional<useStaticMoveEvaluation, ExtMove, Move>::type MoveType;
	typedef MoveList<Move, maxPly> PlyList;
	typedef MoveList<ExtMove, maxMoves> Moves;
	static const bool STATIC_EVALUATION = useStaticMoveEvaluation;
	static const int MAX_PLY = maxPly > 0 ? maxPly : 128;
	static const int MAX_MOVES = maxMoves > 0 ? maxMoves : 128;


	/**
	* Clears the state of the board game by undoing all moves.
	*/
	void clear()
	{
		while (moveCounter() > 0)
		{
			undoMove();
		}
	}

	/**
	* A 64bit hash value for the given position which is used for transposition table.
	* See "Zobrist Hashing" for more information.
	*
	* @return the hash value
	*/
	uint64_t hash() const
	{
		return static_cast<const DerivedBoardGame*>(this)->hash_impl();
	}

	/**
	* Returns the current player which is either 0 or 1.
	* If called during makeMove()/undoMove() it returns the
	* player that was active before.
	*/
	int currentPlayer() const
	{
		return moveCounter() & 1;
	}

	/**
	* Returns the opponent player which is either 0 or 1.
	* If called during makeMove()/undoMove() it returns the
	* opponent player that was active before.
	*/
	int opponentPlayer() const
	{
		return currentPlayer() ^ 1;
	}

	void makeMove(Move n)
	{
		/**
		* Makes a move. It can be assumed that the move is one element of the returned array by moves().
		* AFTER the method returns, the move is added to the ply list
		*
		* @param move a move encoded as an integer
		*/
		static_cast<DerivedBoardGame*>(this)->makeMove_impl(n);
		plyList.addMove(n);
	}
	void undoMove()
	{
		/**
		* Undo a move.
		* After the move is undone, hash() should return the same value as it was before the move was made.
		*  AFTER the method returns, the move is removed from the moveHistory
		*/
		static_cast<DerivedBoardGame*>(this)->undoMove_impl();
		plyList.undoMove();
	}

	/**
	* Returns a new move generator that implements the method "Move nextMove()"
	*/
	auto moveGenerator() const
	{
		return static_cast<const DerivedBoardGame*>(this)->moveGenerator_impl();
	}

	void makeMoves(const std::vector<Move> &moves)
	{
		for (auto move : moves)
		{
			makeMove(move);
		}
	}

	/**
	* Checks if the current game state is a final node. A simple implementation might check if AbstractBoardGame#moves() is empty
	*
	* @return true if no more moves are possible, otherwise false
	*/
	bool isGameOver() const
	{
		return static_cast<const DerivedBoardGame*>(this)->isGameOver_impl();
	}

	/**
	* All possible moves for the current player encoded as an integer and added to the moves array.
	* If the game reached a terminal node, moves shall be left empty.
	*
	* @param moves all possible moves for current player
	*/
	void moves(std::vector<Move>& moves) const
	{
		auto moveGenerator = static_cast<const DerivedBoardGame*>(this)->moveGenerator();
		Move move;
		while ((move = moveGenerator.nextMove()) != noMoveValue())
		{
			moves.push_back(move);
		}
	}

	void moves(Moves& moves) const
	{
		auto moveGenerator = static_cast<const DerivedBoardGame*>(this)->moveGenerator();
		ExtMove move;
		while ((move = moveGenerator.nextMove()) != noMoveValue())
		{
			moves.addMove(move);
		}
	}



	/**
	* A value that evaluates the position from the perspective of the moving player.
	* If it's players 0 turn and player 0 has an advantage over player 1, the method should return a positive value.
	* In the opposite case it should return a negative value.
	* If the position is balanced, 0 is returned.
	* The value must fulfil the following criteria:  abs(value) + MAX_DEPTH < MAX_VALUE. This is required to
	* give the Board game AI the possibility to add the current depth to the value without exceeding the Integer limits!
	*
	* Please note that if player 0 did a move and won the game, it's then player's 1 turn which should return a negative value.
	*
	* @return the evaluation value for the current player
	*/
	int16_t evaluate() const
	{
		return static_cast<const DerivedBoardGame*>(this)->evaluate_impl();
	}
	int16_t evaluate_impl() const
	{
		if (hasWon()) 
		{
			return -(WINNING_VALUE - moveCounter()); //negative because previous player's move won, current player lost
		}
		return 0;
	}

	/**
	* Evaluates if the last move has led to a winning position
	*
	* @return true if the previous player won the game, otherwise false
	*/
	bool hasWon() const
	{
		return static_cast<const DerivedBoardGame*>(this)->hasWon_impl();
	}


	/**
	* The value that represents "no move". This value shall never be returned by moves()
	*
	* @return the "no move" value
	*/
	constexpr static int noMoveValue()
	{
		return DerivedBoardGame::noMoveValue_impl();
	}
	constexpr static int noMoveValue_impl()
	{
		return 0;
	}

	/**
	* The number of moves that were already made.
	*
	* @return number of moves
	*/
	int moveCounter() const
	{
		return (int)plyList.size();
	}

	/**
	* If mate distance pruning is used, this method returns the maximum possible evaluation value for the next moves from the 
	* current position. Usually, this value decreases by one with each move. This value is needed to avoid searching the other moves
	* although a winning move was already found.
	*
	* @return the maximum possible score in the current position
	*/
	int16_t maxPossibleEvaluation() const
	{
		return static_cast<const DerivedBoardGame*>(this)->maxPossibleEvaluation_impl();
	}
	int16_t maxPossibleEvaluation_impl() const
	{
		return WINNING_VALUE - moveCounter() + 1;
	}

	int mapLastMoveToCounterMoveState() const
	{
		return static_cast<const DerivedBoardGame*>(this)->mapLastMoveToCounterMoveState_impl();
	}

	int mapLastMoveToCounterMoveState_impl() const
	{
		return plyList.lastMove();
	}

	/**
	* The size of the history table which is historyTable[2][states]
	*
	* @return the size of the history table for each player
	*/
	constexpr static int counterMoveStates()
	{
		return DerivedBoardGame::counterMoveStates_impl();
	}

protected:
	PlyList plyList;
};