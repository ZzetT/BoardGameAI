#pragma once

#include <iostream>
#include <vector>
#include <array>
#include "AbstractBoardGame.h"

#define WIDTH 7
#define HEIGHT 6
#define H1 (HEIGHT+1)
#define H2 (HEIGHT+2)
#define SIZE (HEIGHT*WIDTH)
#define SIZE1 (H1*WIDTH)
#define ALL1 (((uint64_t)1<<SIZE1)-(uint64_t)1)
#define BOTTOM (ALL1 / COL1) // has bits i*H1 set
#define TOP (BOTTOM << HEIGHT)
#define COL1 (((uint64_t)1<<H1)-(uint64_t)1)
#define HORIZONTAL_BITMASK = 1 << 0 | 1 << 7 | 1 << 14 | 1 << 21;
#define VERTICAL_BITMASK = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;
#define DIAGONAL1_BITMASK = 1 << 0 | 1 << 8 | 1 << 16 | 1 << 24;
#define DIAGONAL2_BITMASK = 1 << 3 | 1 << 9 | 1 << 15 | 1 << 21;


class Connect4Game : public AbstractBoardGame
{
private:
	uint64_t color[2];  // black and white bitboard
	uint64_t hash = 0;
	std::array<int, WIDTH> height; // holds bit index of lowest free square
	static const std::array<std::array<uint64_t, 48>, 2> randomNumbers;
	bool won = false;
	

	bool calculateHaswon();

	bool isPlayable(int col) const;
	friend std::ostream& operator<<(std::ostream&, const Connect4Game&);

public:
	Connect4Game();
	~Connect4Game();
	uint64_t getHash() const override;
	void handleMakeMove(int n) override;
	void handleUndoMove() override;
	bool isGameOver() const override;
	void getMoves(MoveList* moves) const override;
	bool hasWon() const override;

protected:

	

};

