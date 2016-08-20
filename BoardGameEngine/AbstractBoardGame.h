#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "MoveList.h"


class AbstractBoardGame
{
protected:
	MoveList moveHistory;
public:
	int moveCounter = 0; // faster access compared to moveHistory.size()

	AbstractBoardGame();
	~AbstractBoardGame();

	/**
	* A 64bit hash value for the given position which is used for transposition table.
	* See "Zobrist Hashing" for more information.
	*
	* @return the hash value
	*/
	virtual uint64_t getHash() const = 0;

	const MoveList* getMoveHistory() { return &moveHistory; }

	/**
	* Returns the current player which is either 0 or 1.
	* If called during makeMove()/undoMove() it returns the
	* player that was active before.
	*/
	int currentPlayer() const { return moveCounter & 1; }

	/**
	* Returns the opponent player which is either 0 or 1.
	* If called during makeMove()/undoMove() it returns the
	* opponent player that was active before.
	*/
	int opponentPlayer() const { return currentPlayer() ^ 1; }

	virtual void makeMove(int n) final;
	virtual void undoMove() final;

	void makeMoves(std::vector<int> &moves);

	/**
	* Checks if the current game state is a final node. A simple implementation might check if AbstractBoardGame#getMoves() is empty
	*
	* @return true if no more moves are possible, otherwise false
	*/
	virtual bool isGameOver() const = 0;

	/**
	* All possible moves for the current player encoded as an integer and added to the moves array.
	* If the game reached a terminal node, moves shall be left empty.
	*
	* @param moves all possible moves for current player
	*/
	virtual void getMoves(MoveList* moves) const = 0;
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
	virtual int evaluate() { return 0; }

	/**
	* Evaluates if the last move has led to a winning position
	*
	* @return true if the previous player won the game, otherwise false
	*/
	virtual bool hasWon() const = 0;

	static const int WINNING_VALUE = 100000;

protected:
	/**
	* Undo a move.
	* After the move is undone, hash() should return the same value as it was before the move was made.
	*  AFTER the method returns, the move is removed from the moveHistory
	*/
	virtual void handleUndoMove() = 0;

	/**
	* Makes a move. It can be assumed that the move is one element of the returned array by getMoves().
	* AFTER the method returns, the move is added to the moveHistory
	*
	* @param move a move encoded as an integer
	*/
	virtual void handleMakeMove(int n) = 0;

};

