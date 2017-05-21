
#include "Connect4Game.h"
#include "BoardGameAI.h"
#include "StatusUpdate.h"
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

class CoutStatus : public StatusUpdate
{
	void sendCurrentDepth(int depth) override
	{
		cout << "\ranalyzing depth: " << depth << std::flush;
	}
	void sendPrelimaryResult(unsigned int depth, int bestMove, int16_t score) override
	{
		cout << " best move: " << bestMove << " score: " << score << std::flush;
	}

};

int main()
{
	CoutStatus s;
	std::shared_ptr<Connect4Game> game = std::make_shared<Connect4Game>();
	std::vector<Move> moves;
	auto ai = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().enableTimeManagement().useMateDistancePruning().updateStats().create(&s);
	string input;

	cout << "1-7: move c: ai move" << endl;
	cout << game->toString() << endl;
	do
	{
		int col;
		int value;
		bool kiMove;
		do
		{
			kiMove = false;
			cin >> input;
			if (input == string("c"))
			{
				cout << "AI is calculating next move..." << endl;
				SearchResult result = ai.search(game, INT_MAX, 3000);
				game->makeMove(result.bestMove);
				value = result.value;
				kiMove = true;
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
					moves.clear();
					game->moves(moves);
					if (find_if(moves.begin(), moves.end(), [&col](const Move& obj) {return obj == col; }) != moves.end())
					{
						game->makeMove(col);
						break;
					}
				}
			}

		} while (true);
		system("cls");
		cout << "1-7: move c: ai move" << endl;
		cout << game->toString() << endl;
		if (kiMove)
		{
			std::cout << "value: " << value << std::endl;
		}
	} while (!game->isGameOver());
	system("pause");
	return 0;
}

