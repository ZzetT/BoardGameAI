#include "stdafx.h"
#include "TestGame.h"
#include "CppUnitTest.h"

const int evaluation[3][2][3] = { {{10, -5, 3}, {-6, 12, 99}}, {{10,12,3}, {13, 99, 99}}, {{3, 2, -4}, {99, 99, 99}} };

const char nodeChar[] = { 'A', 'B', 'C' };

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TestGame::TestGame()
{
}


TestGame::~TestGame()
{
}

uint64_t TestGame::getHash() const
{
	return std::hash<std::string>()(nodeName);
}


void TestGame::handleMakeMove(Move n)
{
	moveHistory[depth] = n;
	nodeName.push_back('A' + n);
	depth++;
}

void TestGame::handleUndoMove()
{
	depth--;
	nodeName.pop_back();
}

bool TestGame::isGameOver() const
{
	return depth == 3;
}

void TestGame::getMoves(MoveList* moves) const
{
	moves->addMove(0);
	moves->addMove(1);
	if (depth != 1)
	{
		moves->addMove(2);
	}
}

int16_t TestGame::evaluate()
{
	// simulate tree from https://de.wikipedia.org/wiki/Alpha-Beta-Suche#/media/File:Alpha_beta.png
	int value = 0;
	if (depth == 3)
	{
		value = -evaluation[moveHistory[0]][moveHistory[1]][moveHistory[2]];
		Assert::AreNotEqual(99, value);
	}
	return value;
}


