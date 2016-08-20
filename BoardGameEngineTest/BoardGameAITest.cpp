#include "stdafx.h"
#include "BoardGameAI.h"
#include "TestGame.h"
#include "TraceOutput.h"
#include <map>
#include <iostream>
#include "CppUnitTest.h"
#include "Connect4Game.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define h(x) std::hash<std::string>()(x)


namespace BoardGameEngineTest
{
	TEST_CLASS(BoardGameAITest)
	{
	public:

		/* R [-inf, 12, inf]
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
		TEST_METHOD(search)
		{
			std::shared_ptr<TestGame> game = std::make_shared<TestGame>();
			BoardGameAI<true> ai;
			std::shared_ptr<TraceOutput> traceOutput = std::make_shared<TraceOutput>();
			ai.setTrace(traceOutput);
			int bestMove = ai.search(game, 3);
			Assert::AreEqual(1, bestMove);
			
			int inf = BoardGameAI<true>::INFINITE;

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

			Assert::IsTrue(traceOutput->matchExact(3, &root));

		}

		/**
		* Checks if the AI uses the move with the quickest win
		*/
		TEST_METHOD(quickestWin)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			BoardGameAI<> ai;
			std::vector<int> moves = {1,0,1,0,1,1,0,0,2,0,3,4,3,3,2,4,2,2,1};
			game->makeMoves(moves);
			int bestMove = ai.search(game, 6);

			Assert::IsTrue(bestMove == 3 || bestMove == 5);

		}
		TEST_METHOD(game5)
		{
			std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
			BoardGameAI<> ai;
			std::vector<int> moves = { 3,4,0,2,2,3,3,2,2,4,2,0,0,0,4,4,4,6,6,6,6,6,4,6 };
			game->makeMoves(moves);

			int bestMove = ai.search(game, 20);
			Assert::IsTrue(bestMove <= 2);


		}

	};
}