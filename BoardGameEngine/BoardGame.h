#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <cassert>
#include "MoveList.h"
#include "types.h"

template <typename DerivedBoardGame>
class BoardGame
{
protected:
	MoveList moveHistory;
public:
	int moveCounter = 0; // faster access compared to moveHistory.size()

	/**
	* A 64bit hash value for the given position which is used for transposition table.
	* See "Zobrist Hashing" for more information.
	*
	* @return the hash value
	*/
	uint64_t getHash() const
	{
		return static_cast<const DerivedBoardGame*>(this)->getHash_impl();
	}

	/**
	* Returns the list of moves that were already made
	*
	* @return the move list
	*/
	const MoveList* getMoveHistory()
	{
		return &moveHistory;
	}

	/**
	* Returns the current player which is either 0 or 1.
	* If called during makeMove()/undoMove() it returns the
	* player that was active before.
	*/
	int currentPlayer() const
	{
		return moveCounter & 1;
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
		* Makes a move. It can be assumed that the move is one element of the returned array by getMoves().
		* AFTER the method returns, the move is added to the moveHistory
		*
		* @param move a move encoded as an integer
		*/
		static_cast<DerivedBoardGame*>(this)->makeMove_impl(n);
		moveHistory.addMove(n);
		moveCounter++;
	}
	void undoMove()
	{
		/**
		* Undo a move.
		* After the move is undone, hash() should return the same value as it was before the move was made.
		*  AFTER the method returns, the move is removed from the moveHistory
		*/
		static_cast<DerivedBoardGame*>(this)->undoMove_impl();
		moveHistory.undoMove();
		moveCounter--;
	}

	void makeMoves(std::vector<Move> &moves)
	{
		for (auto move : moves)
		{
			makeMove(move);
		}
	}

	/**
	* Checks if the current game state is a final node. A simple implementation might check if AbstractBoardGame#getMoves() is empty
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
	void getMoves(MoveList* moves) const
	{
		return static_cast<const DerivedBoardGame*>(this)->getMoves_impl(moves);
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
	* Maps the given move to a state in the history table (historyTable[2][state])
	*
	* @return the state in the history table. Value must not be greater than HISTORY_STATES
	*/
	int mapMoveToHistoryState(Move move) const
	{
		return static_cast<const DerivedBoardGame*>(this)->mapMoveToHistoryState_impl(move);
	}

	/**
	* The size of the history table which is historyTable[2][states]
	*
	* @return the size of the history table for each player
	*/
	constexpr static int getHistoryStates()
	{
		return DerivedBoardGame::getHistoryStates_impl();
	}

	/**
	* The value that represents "no move". This value shall never be returned by getMoves()
	*
	* @return the "no move" value
	*/
	constexpr static int getNoMoveValue()
	{
		return DerivedBoardGame::getNoMoveValue_impl();
	}
	constexpr static int getNoMoveValue_impl()
	{
		return 0;
	}

};