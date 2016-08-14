#include "stdafx.h"
#include "Connect4Game.h"
#include "CppUnitTest.h"
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BoardGameEngineTest
{		
	TEST_CLASS(Connect4GameTest)
	{
	public:
		
		TEST_METHOD(CurrentPlayer)
		{
			Connect4Game game;
			Assert::AreEqual(0, game.currentPlayer());
			game.makeMove(3);
			Assert::AreEqual(1, game.currentPlayer());
			game.makeMove(4);
			Assert::AreEqual(0, game.currentPlayer());
			game.undoMove();
			Assert::AreEqual(1, game.currentPlayer());
			game.undoMove();
			Assert::AreEqual(0, game.currentPlayer());
		}
		TEST_METHOD(GetMoves)
		{
			Connect4Game game;
			MoveList moves;
			moves.clear();
			game.getMoves(&moves);
			Assert::AreEqual((size_t)7, moves.size());
			for (int i = 0; i < 6; i++) {
				game.makeMove(0);
			}
			moves.clear();
			game.getMoves(&moves);
			Assert::AreEqual((size_t)6, moves.size());

			for (int i = 0; i < 6; i++) {
				game.makeMove(1);
			}
			moves.clear();
			game.getMoves(&moves);
			Assert::AreEqual((size_t)5, moves.size());

			for (int i = 0; i < 6; i++) {
				game.makeMove(2);
			}

			moves.clear();
			game.getMoves(&moves);
			Assert::AreEqual((size_t)4, moves.size());

			game.makeMove(6);
			for (int i = 0; i < 6; i++) {
				game.makeMove(3);
			}

			moves.clear();
			game.getMoves(&moves);
			Assert::AreEqual((size_t)3, moves.size());

			for (int i = 0; i < 6; i++) {
				game.makeMove(4);
			}

			moves.clear();
			game.getMoves(&moves);
			Assert::AreEqual((size_t)2, moves.size());

			game.makeMove(6);
			for (int i = 0; i < 6; i++) {
				game.makeMove(5);
			}

			moves.clear();
			game.getMoves(&moves);
			Assert::AreEqual((size_t)1, moves.size());

			for (int i = 0; i < 4; i++) {
				game.makeMove(6);
			}

			moves.clear();
			game.getMoves(&moves);
			Assert::AreEqual((size_t)0, moves.size());
		}

		TEST_METHOD(HasWonVertical)
		{
			Connect4Game game;
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(3);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(2);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(3);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(2);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(3);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(2);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(3);
			Assert::AreEqual(-AbstractBoardGame::WINNING_VALUE, game.evaluate());
			Assert::IsTrue(game.isGameOver());
		}
		TEST_METHOD(HasWonHorizontal)
		{
			Connect4Game game;
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(3);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(3);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(2);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(2);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(1);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(1);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(5);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(0);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(5);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(0);
			Assert::AreEqual(-AbstractBoardGame::WINNING_VALUE, game.evaluate());
			Assert::IsTrue(game.isGameOver());
		}
		TEST_METHOD(HasWonDiagonal)
		{
			Connect4Game game;
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(5);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(4);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(4);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(3);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(3);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(0);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(3);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(2);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(2);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(2);
			Assert::AreEqual(0, game.evaluate());
			game.makeMove(2);
			Assert::AreEqual(-AbstractBoardGame::WINNING_VALUE, game.evaluate());
			Assert::IsTrue(game.isGameOver());
		}
		TEST_METHOD(Hash)
		{
			Connect4Game game;
			Assert::AreEqual((uint64_t)0, game.getHash());
			game.makeMove(2);
			Assert::AreEqual((uint64_t)2599907046820534596, game.getHash());
			game.makeMove(6);
			Assert::AreEqual((uint64_t)7916255078273704207, game.getHash());
			game.makeMove(2);
			Assert::AreEqual((uint64_t)5376913096070052708, game.getHash());
		}

	};
}