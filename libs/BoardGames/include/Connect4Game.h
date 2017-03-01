#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include "BoardGame.h"


class Connect4Game : public BoardGame<Connect4Game, 8, 50>
{
private:
	static const int WIDTH = 7;
	static const int HEIGHT = 6;
	static const int H1 = HEIGHT+1;
	static const int H2 = HEIGHT + 2;
	static const int SIZE = HEIGHT*WIDTH;
	static const int SIZE1 = H1*WIDTH;
	static const uint64_t ALL1 = ((uint64_t)1 << SIZE1) - (uint64_t)1;
	static const int COL1 = (1 << H1) - 1;
	static const uint64_t BOTTOM = ALL1 / COL1;
	static const uint64_t TOP = BOTTOM << HEIGHT;


	uint64_t color[2];  // black and white bitboard
	uint64_t hash = 0;
	std::array<int, WIDTH+1> height; // holds bit index of lowest free square
	static const std::array<std::array<uint64_t, 48>, 2> randomNumbers;
	static const std::array<int, 8> Connect4Game::colValues;
	bool won = false;

	bool calculateHaswon();

	bool isPlayable(int col) const;
	friend std::ostream& operator<<(std::ostream&, const Connect4Game&);

public:
	static const int HISTORY_STATES = SIZE1;

	Connect4Game();
	~Connect4Game();
	uint64_t getHash_impl() const;
	void makeMove_impl(Move n);
	void undoMove_impl();
	bool isGameOver_impl() const;
	void getMoves_impl(MovesPerPositionType* moves) const;
	bool hasWon_impl() const;
	int mapMoveToHistoryState_impl(Move move) const;
	constexpr static int getHistoryStates_impl()
	{
		return SIZE1;
	}

	std::string toString()
	{
		std::stringstream ss;
		ss << this;
		return ss.str();
	}

};

