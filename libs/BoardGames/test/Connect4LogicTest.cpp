#include "Connect4Game.h"

#include "catch.hpp"

TEST_CASE("Connect4: Current player", "[Connect4][Logic]")
{
	Connect4Game game;
	REQUIRE(0 == game.currentPlayer());
	game.makeMove(3);
	REQUIRE(1 == game.currentPlayer());
	game.makeMove(4);
	REQUIRE(0 == game.currentPlayer());
	game.undoMove();
	REQUIRE(1 == game.currentPlayer());
	game.undoMove();
	REQUIRE(0 == game.currentPlayer());
}

TEST_CASE("Connect4: Get Moves", "[Connect4][Logic]")
{
	Connect4Game game;
	std::vector<Move> moves;
	moves.clear();
	game.moves(moves);
	REQUIRE((size_t)7 == moves.size());
	for (int i = 0; i < 6; i++) {
		game.makeMove(1);
	}
	moves.clear();
	game.moves(moves);
	REQUIRE((size_t)6 == moves.size());

	for (int i = 0; i < 6; i++) {
		game.makeMove(2);
	}
	moves.clear();
	game.moves(moves);
	REQUIRE((size_t)5 == moves.size());

	for (int i = 0; i < 6; i++) {
		game.makeMove(3);
	}

	moves.clear();
	game.moves(moves);
	REQUIRE((size_t)4 == moves.size());

	game.makeMove(7);
	for (int i = 0; i < 6; i++) {
		game.makeMove(4);
	}

	moves.clear();
	game.moves(moves);
	REQUIRE((size_t)3 == moves.size());

	for (int i = 0; i < 6; i++) {
		game.makeMove(5);
	}

	moves.clear();
	game.moves(moves);
	REQUIRE((size_t)2 == moves.size());

	game.makeMove(7);
	for (int i = 0; i < 6; i++) {
		game.makeMove(6);
	}

	moves.clear();
	game.moves(moves);
	REQUIRE((size_t)1 == moves.size());

	for (int i = 0; i < 4; i++) {
		game.makeMove(7);
	}

	moves.clear();
	game.moves(moves);
	REQUIRE((size_t)0 == moves.size());
}
TEST_CASE("Connect4: Has Won Vertical", "[Connect4][Logic]")
{
	Connect4Game game;
	REQUIRE(!game.hasWon());
	game.makeMove(4);
	REQUIRE(!game.hasWon());
	game.makeMove(3);
	REQUIRE(!game.hasWon());
	game.makeMove(4);
	REQUIRE(!game.hasWon());
	game.makeMove(3);
	REQUIRE(!game.hasWon());
	game.makeMove(4);
	REQUIRE(!game.hasWon());
	game.makeMove(3);
	REQUIRE(!game.hasWon());
	game.makeMove(4);
	game.hasWon();
	REQUIRE(game.isGameOver());
}
TEST_CASE("Connect4: Has Won Horizontal", "[Connect4][Logic]")
{
	Connect4Game game;
	REQUIRE(!game.hasWon());
	game.makeMove(4);
	REQUIRE(!game.hasWon());
	game.makeMove(4);
	REQUIRE(!game.hasWon());
	game.makeMove(3);
	REQUIRE(!game.hasWon());
	game.makeMove(3);
	REQUIRE(!game.hasWon());
	game.makeMove(2);
	REQUIRE(!game.hasWon());
	game.makeMove(2);
	REQUIRE(!game.hasWon());
	game.makeMove(6);
	REQUIRE(!game.hasWon());
	game.makeMove(1);
	REQUIRE(!game.hasWon());
	game.makeMove(6);
	REQUIRE(!game.hasWon());
	game.makeMove(1);
	game.hasWon();
	REQUIRE(game.isGameOver());
}
TEST_CASE("Connect4: Has Won Diagonal", "[Connect4][Logic]")
{
	Connect4Game game;
	REQUIRE(!game.hasWon());
	game.makeMove(6);
	REQUIRE(!game.hasWon());
	game.makeMove(5);
	REQUIRE(!game.hasWon());
	game.makeMove(5);
	REQUIRE(!game.hasWon());
	game.makeMove(4);
	REQUIRE(!game.hasWon());
	game.makeMove(4);
	REQUIRE(!game.hasWon());
	game.makeMove(1);
	REQUIRE(!game.hasWon());
	game.makeMove(4);
	REQUIRE(!game.hasWon());
	game.makeMove(3);
	REQUIRE(!game.hasWon());
	game.makeMove(3);
	REQUIRE(!game.hasWon());
	game.makeMove(3);
	REQUIRE(!game.hasWon());
	game.makeMove(3);
	game.hasWon();
	REQUIRE(game.isGameOver());
}
TEST_CASE("Connect4: Hash", "[Connect4][Logic]")
{
	Connect4Game game;
	REQUIRE(0ULL == game.hash());
	game.makeMove(3);
	REQUIRE(2599907046820534596ULL == game.hash());
	game.makeMove(7);
	REQUIRE(7916255078273704207ULL == game.hash());
	game.makeMove(3);
	REQUIRE(5376913096070052708ULL == game.hash());
}