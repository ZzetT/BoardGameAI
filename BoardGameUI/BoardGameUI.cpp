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
	BoardGameAI<> ai;
	string input;

	cout << *game << endl;
	do
	{
		int col;
		do
		{
			cin >> input;
			if (input == string("c"))
			{
				cout << "AI is calculating next move..." << endl;
				int bestMove = ai.search(game, INT_MAX, 2000);
				game->makeMove(bestMove);
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
					if (find_if(moves->begin(), moves->end(), [&col](const Move& obj) {return obj.move == col; }) != moves->end())
					{
						game->makeMove(col);
						break;
					}
				}
			}

		} while (true);
		system("cls");
		cout << *game << endl;
	} while (!game->isGameOver());
	system("pause");
	return 0;
}

