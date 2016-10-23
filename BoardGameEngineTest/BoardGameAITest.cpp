#include "stdafx.h"
#include "BoardGameAI.h"
#include "TestGame.h"
#include "TraceOutput.h"
#include <map>
#include <iostream>
#include "CppUnitTest.h"
#include "Connect4Game.h"
#include "DebugTrace.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace BoardGameEngineTest
{
	TEST_CLASS(BoardGameAITest)
	{
	public:

		/* The following test is taken from https://de.wikipedia.org/wiki/Alpha-Beta-Suche#/media/File:Alpha_beta.png
		* R [-inf, 12, inf]
		* |-R0 [-inf, -10, inf]
		*   |-R00 [-inf, 10, inf]
		*     |-R000 -10
		*     |-R001 5
		*     |-R002 -3
		*   |-R01 [-inf, 12, 10]
		*     |-R010 6
		*     |-R011 -12
		*     |-R012 cutoff
		* |-R1 [-inf, -12, -10]
		*   |-R10 [10, 12, inf]
		*     |-R100 -10
		*     |-R101 -12
		*     |-R102 -3
		*   |-R11 [10, 13, 12]
		*     |-R110 -13
		*     |-R111 cutoff
		*     |-R112 cutoff
		* |-R2 [-inf, -3, -12]
		*   |-R20 [12, 3, inf]
		*     |-R200 -3
		*     |-R201 -2
		*     |-R202 4
		*   |-R21 cutoff
		*     |-R210 cutoff
		*     |-R211 cutoff
		*     |-R212 cutoff
		*/
		TEST_METHOD(SIMPLE_NEGAMAX_TREE)
		{
			std::shared_ptr<TestGame> game = std::make_shared<TestGame>();
			auto ai = AIBuilder{}.doTracing().setMoveNone<0xFF>().create();
			std::shared_ptr<TraceOutput> traceOutput = std::make_shared<TraceOutput>();
			ai.setTrace(traceOutput);
			int bestMove = ai.search(game, 3).bestMove;
			Assert::AreEqual(1, bestMove);
			
			int inf = INFINITE;

			TraceOutputNode root = TraceOutputNode(12, -inf, inf);
			root[0][0][0].set(-10);
			root[0][0][1].set(5);
			root[0][0][2].set(-3);
			root[0][0].set(10, -inf, inf);

			root[0][1][0].set(6);
			root[0][1][1].set(-12);
			root[0][1].set(12, -inf, 10);

			root[1][0][0].set(-10);
			root[1][0][1].set(-12);
			root[1][0][2].set(-3);
			root[1][0].set(12, 10, inf);

			root[1][1][0].set(-13);
			root[1][1].set(13, 10, 12);

			root[2][0][0].set(-3);
			root[2][0][1].set(-2);
			root[2][0][2].set(4);
			root[2][0].set(3, 12, inf);

			root[0].set(-10, -inf, inf);
			root[1].set(-12, -inf, -10);
			root[2].set(-3, -inf, -12);

			std::stringstream ss;
			ss << *traceOutput;
			std::string name = ss.str();
			Logger::WriteMessage(name.c_str());

			Assert::IsTrue(traceOutput->matchExact(1, &root));

		}

		/**
		* Checks if the AI uses the move with the quickest win
		*/
		TEST_METHOD(quickestWin)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.iterativeDeepening().create();
			std::vector<Move> moves = {2,1,2,1,2,2,1,1,3,1,4,5,4,4,3,5,3,3,2};
			game->makeMoves(moves);
			int bestMove = ai.search(game, 6).bestMove;
			Assert::IsTrue(bestMove == 4 || bestMove == 6);
		}
		TEST_METHOD(AB_MATE_IN_3)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.create();
			std::vector<Move> moves = { 4,4,4,4,4,5,5,5,5,2,5,2,2,1,1,1,1,7,7,4,5,7,7,7,7,1,1,6,6,6,6,6,3,3,3,3 };
			game->makeMoves(moves);
			SearchResult result = ai.search(game);
			Assert::IsTrue(result.bestMove == 3);
			Assert::IsTrue(result.value == WINNING_VALUE - (game->moveCounter + 5)); // win in 5 half moves
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 53);
		}

		TEST_METHOD(ID_TT_MATE_IN_3)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.iterativeDeepening().useTTable().create();
			std::vector<Move> moves = { 4,4,4,4,4,5,5,5,5,2,5,2,2,1,1,1,1,7,7,4,5,7,7,7,7,1,1,6,6,6,6,6,3,3,3,3 };
			game->makeMoves(moves);
			SearchResult result = ai.search(game);
			Assert::IsTrue(result.bestMove == 3);
			Assert::IsTrue(result.value == WINNING_VALUE -(game->moveCounter + 5)); // win in 5 half moves
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 5393);
		}


		TEST_METHOD(AB_MATE_IN_5)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.create();
			std::vector<Move> moves = { 4,3,6,5,5,5,3,4,4,5,4,4,5,6,1,7,7,7,2,2,2,2,3,3,2,6,6,3 };
			game->makeMoves(moves);
			SearchResult result = ai.search(game);
			Assert::IsTrue(result.bestMove == 3);
			Assert::IsTrue(result.value == WINNING_VALUE - (game->moveCounter + 9)); // win in 5 half moves
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 2439);
		}
		TEST_METHOD(ID_TT_MATE_IN_5)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.useTTable().iterativeDeepening().create();
			std::vector<Move> moves = { 4,3,6,5,5,5,3,4,4,5,4,4,5,6,1,7,7,7,2,2,2,2,3,3,2,6,6,3 };
			game->makeMoves(moves);
			SearchResult result = ai.search(game);
			Assert::IsTrue(result.bestMove == 3);
			Assert::IsTrue(result.value == WINNING_VALUE - (game->moveCounter + 9)); // win in 5 half moves
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 90316);
		}
		TEST_METHOD(AB_MATE_IN_6)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.create();
			std::vector<Move> moves = { 2,4,4,4,2,4,2,2,4,4,5,3,3, };
			game->makeMoves(moves);
			SearchResult result = ai.search(game);
			Assert::IsTrue(result.bestMove == 5);
			game->makeMove(5);
			game->makeMove(5);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 5);
			game->makeMove(5);
			game->makeMove(5);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 5);
			game->makeMove(5);
			game->makeMove(7);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 2);
			game->makeMove(2);
			game->makeMove(7);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 2);
			game->makeMove(2);
			game->makeMove(1);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 1);
			game->makeMove(1);
			game->makeMove(1);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 6);
			game->makeMove(6);
			game->makeMove(6);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 6);
			game->makeMove(6);
			game->makeMove(1);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 6);
			game->makeMove(6);
			game->makeMove(1);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 1);
			game->makeMove(1);
			game->makeMove(6);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 6);
			game->makeMove(6);
			game->makeMove(3);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 3);
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 1284877506);
		}
		TEST_METHOD(ID_TT_MATE_IN_6)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.iterativeDeepening().useTTable().create();
			std::vector<Move> moves = { 2,4,4,4,2,4,2,2,4,4,5,3,3, };
			game->makeMoves(moves);
			SearchResult result = ai.search(game);
			Assert::IsTrue(result.bestMove == 5);
			game->makeMove(5);
			game->makeMove(5);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 5);
			game->makeMove(5);
			game->makeMove(5);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 5);
			game->makeMove(5);
			game->makeMove(7);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 2);
			game->makeMove(2);
			game->makeMove(7);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 2);
			game->makeMove(2);
			game->makeMove(1);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 1);
			game->makeMove(1);
			game->makeMove(1);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 6);
			game->makeMove(6);
			game->makeMove(6);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 6);
			game->makeMove(6);
			game->makeMove(1);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 6);
			game->makeMove(6);
			game->makeMove(1);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 1);
			game->makeMove(1);
			game->makeMove(6);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 6);
			game->makeMove(6);
			game->makeMove(3);
			result = ai.search(game);
			Assert::IsTrue(result.bestMove == 3);
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 256492233);
		}

		TEST_METHOD(ID_TT_game1)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.iterativeDeepening().useTTable().create();
			std::vector<Move> moves = { 4,4,4,3,3,3,7,4,1,3,7,3,3 };
			game->makeMoves(moves);

			SearchResult result = ai.search(game, 40);
			Assert::IsTrue(result.bestMove == 7);
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 1751668235);
		}

		TEST_METHOD(ID_TT_game2)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.iterativeDeepening().useTTable().create();
			game->makeMove(1);
			SearchResult result = ai.search(game, 14);
			Assert::IsTrue(result.bestMove == 4);
			game->makeMove(4);
			game->makeMove(7);
			result = ai.search(game, 14);
			Assert::IsTrue(result.bestMove == 4);
			game->makeMove(4);
			game->makeMove(1);
			result = ai.search(game, 3);
			Assert::IsTrue(result.bestMove == 4);
			game->makeMove(4);
			game->makeMove(2);
			result = ai.search(game, 40);
			Assert::IsTrue(result.bestMove == 4);
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 5150262759);
		}

		TEST_METHOD(AB_game5)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.create();
			std::vector<Move> moves = { 4,5,1,3,3,4,4,3,3,5,3,1,1,1,5,5,5,7,7,7,7,7,5,7 };
			game->makeMoves(moves);

			SearchResult result = ai.search(game, 20);
			Assert::IsTrue(result.bestMove <= 3);
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 124997);
		}

		TEST_METHOD(ID_game5)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.iterativeDeepening().create();
			std::vector<Move> moves = { 4,5,1,3,3,4,4,3,3,5,3,1,1,1,5,5,5,7,7,7,7,7,5,7 };
			game->makeMoves(moves);

			SearchResult result = ai.search(game, 20);
			Assert::IsTrue(result.bestMove <= 3);
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 721402);
		}

		TEST_METHOD(ID_TT_game5)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.iterativeDeepening().useTTable().create();
			std::vector<Move> moves = {4,5,1,3,3,4,4,3,3,5,3,1,1,1,5,5,5,7,7,7,7,7,5,7 };
			game->makeMoves(moves);

			SearchResult result = ai.search(game, 20);
			Assert::IsTrue(result.bestMove <= 3);
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 210913);
		}

		TEST_METHOD(AB_TT_START_TILL_DEPTH_15)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.useTTable().create();
			SearchResult result = ai.search(game, 15);
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 13824178);
		}

		TEST_METHOD(ID_TT_START_TILL_DEPTH_15)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.iterativeDeepening().useTTable().create();
			SearchResult result = ai.search(game, 15);
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 3248448);
		}

		TEST_METHOD(ID_TT_START_TILL_DEPTH_20)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			auto ai = AIBuilder{}.iterativeDeepening().useTTable().create();
			SearchResult result = ai.search(game, 20);
			Logger::WriteMessage(std::to_string(ai.getAnalyzedPositions()).c_str());
			Assert::IsTrue(ai.getAnalyzedPositions() == 170615194);
		}

	};
}