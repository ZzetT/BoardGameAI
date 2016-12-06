#include "Connect4Game.h"
#include "BoardGameAI.h"

#include "catch.hpp"


TEST_CASE("Connect4: AB Mate in 3", "[Connect4][AI][AB][MATE3]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.create();
	std::vector<Move> moves = { 4,4,4,4,4,5,5,5,5,2,5,2,2,1,1,1,1,7,7,4,5,7,7,7,7,1,1,6,6,6,6,6,3,3,3,3 };
	game->makeMoves(moves);
	SearchResult result = ai.search(game);
	REQUIRE(result.bestMove == 3);
	REQUIRE(result.value == WINNING_VALUE - (game->moveCounter + 5)); // win in 5 half moves
	REQUIRE(ai.getAnalyzedPositions() == 53);
}

TEST_CASE("Connect4: ID TT Mate in 3", "[Connect4][AI][ID][TTReplaceDepth][MATE3]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().create();
	std::vector<Move> moves = { 4,4,4,4,4,5,5,5,5,2,5,2,2,1,1,1,1,7,7,4,5,7,7,7,7,1,1,6,6,6,6,6,3,3,3,3 };
	game->makeMoves(moves);
	SearchResult result = ai.search(game, 5);
	REQUIRE(result.bestMove == 3);
	REQUIRE(result.value == WINNING_VALUE - (game->moveCounter + 5)); // win in 5 half moves
	REQUIRE(ai.getAnalyzedPositions() == 104);
}

TEST_CASE("Connect4: AB Mate in 5", "[Connect4][AI][AB][MATE5]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.create();
	std::vector<Move> moves = { 4,3,6,5,5,5,3,4,4,5,4,4,5,6,1,7,7,7,2,2,2,2,3,3,2,6,6,3 };
	game->makeMoves(moves);
	SearchResult result = ai.search(game, 9);
	REQUIRE(result.bestMove == 3);
	REQUIRE(result.value == WINNING_VALUE - (game->moveCounter + 9)); // win in 9 half moves
	REQUIRE(ai.getAnalyzedPositions() == 1878);
}

TEST_CASE("Connect4: ID TT Mate in 5", "[Connect4][AI][ID][TTReplaceDepth][MATE5]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.useTTable().iterativeDeepening().create();
	std::vector<Move> moves = { 4,3,6,5,5,5,3,4,4,5,4,4,5,6,1,7,7,7,2,2,2,2,3,3,2,6,6,3 };
	game->makeMoves(moves);
	SearchResult result = ai.search(game, 9);
	REQUIRE(result.bestMove == 3);
	REQUIRE(result.value == WINNING_VALUE - (game->moveCounter + 9)); // win in 9 half moves
	REQUIRE(ai.getAnalyzedPositions() == 2284);
}

TEST_CASE("Connect4: ID TT Mate in 11", "[Connect4][AI][ID][TTReplaceDepth][MATE11]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.useTTable().iterativeDeepening().create();
	std::vector<Move> moves = { 4,3,4,4,5,7,5,5,6,3,3,1,2,2,2,2,6,7 };
	game->makeMoves(moves);
	SearchResult result = ai.search(game, 19);
	REQUIRE(result.bestMove == 6);
	REQUIRE(result.value == WINNING_VALUE - (game->moveCounter + 19)); // win in 21 half moves
	REQUIRE(ai.getAnalyzedPositions() == 18075085);
}
TEST_CASE("Connect4: ID TT History Mate in 11", "[Connect4][AI][ID][TTReplaceDepth][HistoryH][MATE11]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.useTTable().iterativeDeepening().useHistoryHeuristic().create();
	std::vector<Move> moves = { 4,3,4,4,5,7,5,5,6,3,3,1,2,2,2,2,6,7 };
	game->makeMoves(moves);
	SearchResult result = ai.search(game, 19);
	REQUIRE(result.bestMove == 6);
	REQUIRE(result.value == WINNING_VALUE - (game->moveCounter + 19)); // win in 21 half moves
	REQUIRE(ai.getAnalyzedPositions() == 6679206);
}

TEST_CASE("Connect4: ID TT Mate in 6", "[Connect4][AI][ID][TTReplaceDepth][MATE6]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().create();
	std::vector<Move> moves = { 2,4,4,4,2,4,2,2,4,4,5,3,3, };
	game->makeMoves(moves);
	SearchResult result = ai.search(game, 23);
	REQUIRE(result.bestMove == 5);
	game->makeMove(5);
	game->makeMove(5);
	result = ai.search(game, 21);
	REQUIRE(result.bestMove == 5);
	game->makeMove(5);
	game->makeMove(5);
	result = ai.search(game, 19);
	REQUIRE(result.bestMove == 5);
	game->makeMove(5);
	game->makeMove(7);
	result = ai.search(game, 17);
	REQUIRE(result.bestMove == 2);
	game->makeMove(2);
	game->makeMove(7);
	result = ai.search(game, 15);
	REQUIRE(result.bestMove == 2);
	game->makeMove(2);
	game->makeMove(1);
	result = ai.search(game, 13);
	REQUIRE(result.bestMove == 1);
	game->makeMove(1);
	game->makeMove(1);
	result = ai.search(game, 11);
	REQUIRE(result.bestMove == 6);
	game->makeMove(6);
	game->makeMove(6);
	result = ai.search(game, 9);
	REQUIRE(result.bestMove == 6);
	game->makeMove(6);
	game->makeMove(1);
	result = ai.search(game, 7);
	REQUIRE(result.bestMove == 6);
	game->makeMove(6);
	game->makeMove(1);
	result = ai.search(game, 5);
	REQUIRE(result.bestMove == 1);
	game->makeMove(1);
	game->makeMove(6);
	result = ai.search(game, 3);
	REQUIRE(result.bestMove == 6);
	game->makeMove(6);
	game->makeMove(3);
	result = ai.search(game, 1);
	REQUIRE(result.bestMove == 3);
	REQUIRE(ai.getAnalyzedPositions() == 11443772);

}

//uncomment with history heuristic
//TEST_CASE("Connect4: ID TT Game 1", "[Connect4][AI][ID][TTReplaceDepth][Game1]")
//{
//	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
//	auto ai = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().create();
//	std::vector<Move> moves = { 4,4,4,3,3,3,7,4,1,3,7,3,3 };
//	game->makeMoves(moves);
//
//	SearchResult result = ai.search(game, 40);
//	REQUIRE(result.bestMove == 7);
//	REQUIRE(ai.getAnalyzedPositions() == 998964036);
//}

//TEST_CASE("Connect4: ID TT Game 2", "[Connect4][AI][ID][TTReplaceDepth][Game2]")
//{
//	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
//	auto ai = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().create();
//	game->makeMove(1);
//	SearchResult result = ai.search(game, 14);
//	REQUIRE(result.bestMove == 4);
//	game->makeMove(4);
//	game->makeMove(7);
//	result = ai.search(game, 14);
//	REQUIRE(result.bestMove == 4);
//	game->makeMove(4);
//	game->makeMove(1);
//	result = ai.search(game, 3);
//	REQUIRE(result.bestMove == 4);
//	game->makeMove(4);
//	game->makeMove(2);
//	result = ai.search(game, 40);
//	REQUIRE(result.bestMove == 4);
//	REQUIRE(ai.getAnalyzedPositions() == 3714091920ULL);
//}

TEST_CASE("Connect4: AB Game 5", "[Connect4][AI][AB]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.create();
	std::vector<Move> moves = { 4,5,1,3,3,4,4,3,3,5,3,1,1,1,5,5,5,7,7,7,7,7,5,7 };
	game->makeMoves(moves);

	SearchResult result = ai.search(game, 20);
	REQUIRE(result.bestMove <= 3);
	REQUIRE(ai.getAnalyzedPositions() == 124997);
}

TEST_CASE("Connect4: ID Game 5", "[Connect4][AI][ID]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.iterativeDeepening().create();
	std::vector<Move> moves = { 4,5,1,3,3,4,4,3,3,5,3,1,1,1,5,5,5,7,7,7,7,7,5,7 };
	game->makeMoves(moves);

	SearchResult result = ai.search(game, 20);
	REQUIRE(result.bestMove <= 3);
	REQUIRE(ai.getAnalyzedPositions() == 596405);
}

TEST_CASE("Connect4: ID TT Game 5", "[Connect4][AI][ID][TTReplaceDepth]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().create();
	std::vector<Move> moves = { 4,5,1,3,3,4,4,3,3,5,3,1,1,1,5,5,5,7,7,7,7,7,5,7 };
	game->makeMoves(moves);

	SearchResult result = ai.search(game, 20);
	REQUIRE(result.bestMove <= 3);
	REQUIRE(ai.getAnalyzedPositions() == 183222);
}

TEST_CASE("Connect4: AB TT Start till depth 15", "[Connect4][AI][AB][TTReplaceDepth][STARTTILL15]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.useTTable().create();
	SearchResult result = ai.search(game, 15);
	REQUIRE(ai.getAnalyzedPositions() == 13488494ULL);
}

TEST_CASE("Connect4: ID TT Start till depth 15", "[Connect4][AI][ID][TTReplaceDepth][STARTTILL15]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().create();
	SearchResult result = ai.search(game, 15);
	REQUIRE(ai.getAnalyzedPositions() == 3237618);
}


TEST_CASE("Connect4: ID TT Start till depth 20", "[Connect4][AI][ID][TTReplaceDepth][STARTTILL20]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().create();
	SearchResult result = ai.search(game, 20);
	REQUIRE(ai.getAnalyzedPositions() == 166509471ULL);
}

TEST_CASE("Connect4: ID TT History Start till depth 20", "[Connect4][AI][ID][TTReplaceDepth][HistoryH]][STARTTILL20]")
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	auto ai = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().useHistoryHeuristic().create();
	SearchResult result = ai.search(game, 20);
	REQUIRE(ai.getAnalyzedPositions() == 1209596621ULL);
	                                     
}
