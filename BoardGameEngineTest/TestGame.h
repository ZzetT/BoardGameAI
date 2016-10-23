#pragma once
#include "AbstractBoardGame.h"
class TestGame :
	public AbstractBoardGame
{
private:
	int depth = 0;
	int moveHistory[3] = { 0 };
	std::string nodeName = "A";

public:
	TestGame();
	~TestGame();

	virtual uint64_t getHash() const override;

	// Inherited via AbstractBoardGame
	virtual void handleMakeMove(Move n) override;
	virtual void handleUndoMove() override;
	virtual bool isGameOver() const override;
	virtual void getMoves(MoveList* moves) const override;
	virtual int16_t evaluate() override;
	virtual bool hasWon() const override {
		return false;
	}
};

