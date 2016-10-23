// BoardGameUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Connect4Game.h"
#include "BoardGameAI.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <climits>

using namespace std;

bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

int main()
{
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	std::unique_ptr<MoveList> moves = std::make_unique<MoveList>();
	auto ai = AIBuilder{}.iterativeDeepening().useTTable().create();
	string input;

	cout << "1-7: move c: ai move" << endl;
	cout << *game << endl;
	do
	{
		int col;
		int winningInMoves = 0;
		do
		{
			cin >> input;
			if (input == string("c"))
			{
				cout << "AI is calculating next move..." << endl;
				SearchResult result = ai.search(game, INT_MAX, 3000);
				game->makeMove(result.bestMove);
				if (result.value >= WINNING_IN_MAX_PLY)
				{
					winningInMoves = (WINNING_VALUE - result.value - game->moveCounter + 1) / 2;
				}
				else if (result.value <= -WINNING_IN_MAX_PLY)
				{
					winningInMoves = -(WINNING_VALUE + result.value - game->moveCounter + 1) / 2;
				}
				else
				{
					winningInMoves = 0;
				}
				break;
			}
			else
			{

				if (!is_number(input))
				{
					cout << "Invalid input. Try again" << endl;
				}
				else
				{
					col = stoi(input);
					moves->clear();
					game->getMoves(moves.get());
					if (find_if(moves->data()->begin(), moves->data()->end(), [&col](const Move& obj) {return obj == col; }) != moves->data()->end())
					{
						game->makeMove(col);
						break;
					}
				}
			}

		} while (true);
		system("cls");
		cout << "1-7: move c: ai move" << endl;
		cout << *game << endl;
		if (std::abs(winningInMoves) > 0)
		{
			std::cout << (game->currentPlayer() ? "o" : "x") << " " << ((winningInMoves > 0) ? "loses" : "wins") << " in " << std::abs(winningInMoves) << " moves" << std::endl;
		}
	} while (!game->isGameOver());
	system("pause");
	return 0;
}

