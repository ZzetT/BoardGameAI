#include "stdafx.h"
#include "Connect4Game.h"
#include "BoardGameAI.h"
#include <string>

using namespace std;

const std::array<std::array<uint64_t, 48>, 2> Connect4Game::randomNumbers = { { { 11142652954828712569, 12963258741717719282, 7530587379591484498, 15417304512952429960, 4451991996897527730, 11723093498588138668, 6031285837589826688, 17432016435118566250, 1056992748988832059, 13707791360677637433, 17385002937540685365, 11840996395816626847, 17565946223180857835, 10179175417957284464, 2599907046820534596, 2828983993981229675, 12907706441128690081, 1725044552147533133, 15911678722628053910, 2526598343422197712, 745096770526208234, 15070242745989399650, 4937931881142388238, 16976579952751209789, 3949905946048527670, 8529551793630656858, 17611392081642658564, 18336405843855253871, 13445685597060895265, 3160678725226440634, 7887191597931285554, 2134088957938657410, 17467967391741297658, 12820128198188026039, 9849771772726820688, 17078139552571680516, 12704693474192801720, 13672237224625523480, 2408192370760125466, 2010295471444340498, 12580017179852689996, 16569411954444982582, 10204420373035765242, 14086611827349033614, 11459867074997075266, 10310538428250479666, 710295030491597007, 15692127572818069872 },{ 7305382856659017700, 8194180939260519198, 9652272581003838483, 4442705013885354341, 3787817380872087257, 15011253020257353980, 9633006481922044616, 11983705658926804063, 3400805992690339125, 787034930067010086, 2125910075593816236, 14970363263950074862, 12972735036281082917, 7789599019143542319, 29578900437220705, 14957773034410429486, 6947976595475714801, 4193596647439353587, 16379224349215073964, 9703605791114182950, 2702415587140522396, 9125596486193986713, 535616561911629950, 11162520439031114684, 18149456365846159322, 14805399633786693379, 858235571035963754, 10810583245903415735, 2582151431000018939, 16615584849919172998, 1866337755096454443, 18424944856003857892, 11444563829175810845, 17265869188025156478, 17632500980770608864, 2584593146783902228, 16482599353588263778, 17413485863375701426, 11804900472302097236, 2657278618889980727, 6959130353562925374, 8549553582166839412, 5316650689343469643, 11370327924826912418, 4891671787225957425, 7677862182423840727, 5289834661413972990, 7048443805870890409 } } };

Connect4Game::Connect4Game()
{
	color[0] = color[1] = 0L;
	for (int i = 0; i<WIDTH; i++)
		height[i] = H1*i;
}


Connect4Game::~Connect4Game()
{
}

uint64_t Connect4Game::getHash() const
{
	return hash;
}

void Connect4Game::handleMakeMove(int n)
{
	color[currentPlayer()] ^= (uint64_t)1L << height[n];
	this->hash ^= randomNumbers[currentPlayer()][height[n]];
	height[n]++;
	this->won = calculateHaswon();
}

void Connect4Game::handleUndoMove()
{
	int n = moveHistory.back();
	--height[n];
	color[opponentPlayer()] ^= (uint64_t)1L << height[n];
	this->hash ^= randomNumbers[opponentPlayer()][height[n]];
	won = false;
}

bool Connect4Game::isGameOver() const
{
	return won || moveCounter == SIZE;
}

bool Connect4Game::haswon() const
{
	return won;
}

bool Connect4Game::isPlayable(int col) const
{
	return ((color[currentPlayer()] | ((uint64_t)1L << height[col])) & TOP) == 0;
}

void Connect4Game::getMoves(MoveList* moves) const
{
	if (won)
	{
		return;
	}
	for (int col = 0; col < 7; col++)
	{
		if (isPlayable(col)) {
			moves->push_back(col);
		}
	}
}

int Connect4Game::evaluate()
{
	if (won) // the previous player won the game
	{
		// previous player won the game
		return -AbstractBoardGame::WINNING_VALUE;
	}
	return 0;
}

bool Connect4Game::calculateHaswon()
{
	uint64_t newboard = color[currentPlayer()];
	uint64_t y = newboard & (newboard >> HEIGHT);
	if ((y & (y >> 2 * HEIGHT)) != 0) // check diagonal "\"
		return true;
	y = newboard & (newboard >> H1);
	if ((y & (y >> 2 * H1)) != 0) // check horizontal -
		return true;
	y = newboard & (newboard >> H2); // check diagonal /
	if ((y & (y >> 2 * H2)) != 0)
		return true;
	y = newboard & (newboard >> 1); // check vertical |
	return (y & (y >> 2)) != 0;
}


std::ostream & operator<<(std::ostream &strm, const Connect4Game &game)
{
	strm << game.getHash() << endl;
	strm << "current moves: ";
	for (auto move : game.moveHistory)
	{
		strm << move << ",";
	}
	strm << endl;
	for (int w = 0; w < WIDTH; w++) {
		if (w > 0)
		{
			strm << " ";
		}
		strm << to_string(w);
	}
	strm << endl;
	for (int h = HEIGHT - 1; h >= 0; h--) {
		bool start = true;
		for (int w = h; w < SIZE1; w += H1) {
			uint64_t mask = (uint64_t)1L << w;
			if (!start)
			{
				strm << " ";
			}
			start = false;
			strm << ((game.color[0] & mask) != 0 ? "x" :
				(game.color[1] & mask) != 0 ? "o" : ".");
		}
		strm << endl;
	}
	if (game.haswon())
		strm << "won" << endl;
	return strm;
}