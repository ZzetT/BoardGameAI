#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include "BoardGame.h"

template<bool staticMoveEvaluation = false, bool staticArray = true>
class Connect4GameExt : public BoardGame<Connect4GameExt<staticMoveEvaluation, staticArray>, staticMoveEvaluation /* true = moves are sorted by AI, false = moves are returned sorted */, 50, staticArray ? 8 : 0>
{
private:
	static const int WIDTH = 7;
	static const int HEIGHT = 6;
	static const int H1 = HEIGHT + 1;
	static const int H2 = HEIGHT + 2;
	static const int SIZE = HEIGHT*WIDTH;
	static const int SIZE1 = H1*WIDTH;
	static const uint64_t ALL1 = ((uint64_t)1 << SIZE1) - (uint64_t)1;
	static const int COL1 = (1 << H1) - 1;
	static const uint64_t BOTTOM = ALL1 / COL1;
	static const uint64_t TOP = BOTTOM << HEIGHT;
	static constexpr std::array<std::array<uint64_t, 48>, 2> randomNumbers = { { { { 11142652954828712569ULL, 12963258741717719282ULL, 7530587379591484498ULL, 15417304512952429960ULL, 4451991996897527730ULL, 11723093498588138668ULL, 6031285837589826688ULL, 17432016435118566250ULL, 1056992748988832059ULL, 13707791360677637433ULL, 17385002937540685365ULL, 11840996395816626847ULL, 17565946223180857835ULL, 10179175417957284464ULL, 2599907046820534596ULL, 2828983993981229675ULL, 12907706441128690081ULL, 1725044552147533133ULL, 15911678722628053910ULL, 2526598343422197712ULL, 745096770526208234ULL, 15070242745989399650ULL, 4937931881142388238ULL, 16976579952751209789ULL, 3949905946048527670ULL, 8529551793630656858ULL, 17611392081642658564ULL, 18336405843855253871ULL, 13445685597060895265ULL, 3160678725226440634ULL, 7887191597931285554ULL, 2134088957938657410ULL, 17467967391741297658ULL, 12820128198188026039ULL, 9849771772726820688ULL, 17078139552571680516ULL, 12704693474192801720ULL, 13672237224625523480ULL, 2408192370760125466ULL, 2010295471444340498ULL, 12580017179852689996ULL, 16569411954444982582ULL, 10204420373035765242ULL, 14086611827349033614ULL, 11459867074997075266ULL, 10310538428250479666ULL, 710295030491597007ULL, 15692127572818069872ULL } },{ { 7305382856659017700ULL, 8194180939260519198ULL, 9652272581003838483ULL, 4442705013885354341ULL, 3787817380872087257ULL, 15011253020257353980ULL, 9633006481922044616ULL, 11983705658926804063ULL, 3400805992690339125ULL, 787034930067010086ULL, 2125910075593816236ULL, 14970363263950074862ULL, 12972735036281082917ULL, 7789599019143542319ULL, 29578900437220705ULL, 14957773034410429486ULL, 6947976595475714801ULL, 4193596647439353587ULL, 16379224349215073964ULL, 9703605791114182950ULL, 2702415587140522396ULL, 9125596486193986713ULL, 535616561911629950ULL, 11162520439031114684ULL, 18149456365846159322ULL, 14805399633786693379ULL, 858235571035963754ULL, 10810583245903415735ULL, 2582151431000018939ULL, 16615584849919172998ULL, 1866337755096454443ULL, 18424944856003857892ULL, 11444563829175810845ULL, 17265869188025156478ULL, 17632500980770608864ULL, 2584593146783902228ULL, 16482599353588263778ULL, 17413485863375701426ULL, 11804900472302097236ULL, 2657278618889980727ULL, 6959130353562925374ULL, 8549553582166839412ULL, 5316650689343469643ULL, 11370327924826912418ULL, 4891671787225957425ULL, 7677862182423840727ULL, 5289834661413972990ULL, 7048443805870890409ULL } } } };



	uint64_t color[2];  // black and white bitboard
	uint64_t mHash = 0;
	std::array<int, WIDTH + 1> height; // holds bit index of lowest free square
	bool won = false;

	bool calculateHaswon()
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


	bool isPlayable(int col) const
	{
		return ((color[currentPlayer()] | ((uint64_t)1L << height[col])) & TOP) == 0;
	}

public:
	Connect4GameExt()
	{
		color[0] = color[1] = 0L;
		for (int i = 0; i < WIDTH; i++)
			height[i + 1] = H1*i;
	}
	~Connect4GameExt() {};
	uint64_t hash_impl() const
	{
		return mHash;
	}
	void makeMove_impl(Move n)
	{
		color[currentPlayer()] ^= (uint64_t)1L << height[n];
		this->mHash ^= randomNumbers[currentPlayer()][height[n]];
		height[n]++;
		this->won = calculateHaswon();
	}
	void undoMove_impl()
	{
		int n = plyList.lastMove();
		--height[n];
		color[opponentPlayer()] ^= (uint64_t)1L << height[n];
		this->mHash ^= randomNumbers[opponentPlayer()][height[n]];
		won = false;
	}
	bool isGameOver_impl() const
	{
		return won || moveCounter() == SIZE;
	}

	bool hasWon_impl() const
	{
		return won;
	}
	int mapLastMoveToCounterMoveState_impl() const
	{
		return height[plyList.lastMove()]-1;
	}

	constexpr static int counterMoveStates_impl()
	{
		return SIZE1;
	}

	int16_t evaluate_impl() const
	{
		if (hasWon_impl())
		{
			return -((SIZE - moveCounter() + 2) / 2);
		}
		return 0;

	}
	int16_t maxPossibleEvaluation_impl() const
	{
		return ((SIZE - moveCounter() + 1) / 2);
	}

	template<bool> class MoveGeneratorBase {};

	// return moves with a value (AI sorts them by value)
	template<>
	class MoveGeneratorBase<true>
	{
	private:
		const Connect4GameExt* game;
		Move move;
		static constexpr std::array<int, 8> colValues = { { 0,1,2,3,4,3,2,1 } };
		static const int INVALID_INDEX = 8;
	public:
		MoveGeneratorBase() {}
		constexpr MoveGeneratorBase(const Connect4GameExt* game) : game(game), move(1) {}
		ExtMove nextMove()
		{
			Move currentMove;
			while ((currentMove = move++) < INVALID_INDEX)
			{
				if (game->isPlayable(currentMove))
				{
					return ExtMove(currentMove, colValues[currentMove]);
				}
			}
			return noMoveValue();
		}

	};

	// or return ordered moves directly
	template<>
	class MoveGeneratorBase<false>
	{
	private:
		const Connect4GameExt* game;
		int index;
		static constexpr std::array<Move, 7> orderedMoves = { { 4,3,5,2,6,1,7 } };
	public:
		MoveGeneratorBase() {}
		constexpr MoveGeneratorBase(const Connect4GameExt* game) : game(game), index(0) {}
		Move nextMove()
		{
			int currentIndex;
			while ((currentIndex = index++) < orderedMoves.size())
			{
				if (game->isPlayable(orderedMoves[currentIndex]))
				{
					return orderedMoves[currentIndex];
				}
			}
			return noMoveValue();
		}

	};

	using MoveGenerator = MoveGeneratorBase<STATIC_EVALUATION>;

	MoveGenerator moveGenerator_impl() const
	{
		return MoveGenerator(this);
	}


	std::string toString()
	{
		std::stringstream strm;
		//strm << "hash: " << game.getHash() << endl;
		strm << "current moves: ";
		for (auto move : *plyList.data())
		{
			strm << move << ",";
		}
		strm << std::endl;
		for (int w = 1; w <= WIDTH; w++) {
			if (w > 1)
			{
				strm << " ";
			}
			strm << std::to_string(w);
		}
		strm << std::endl;
		for (int h = HEIGHT - 1; h >= 0; h--) {
			bool start = true;
			for (int w = h; w < SIZE1; w += H1) {
				uint64_t mask = (uint64_t)1L << w;
				if (!start)
				{
					strm << " ";
				}
				start = false;
				strm << ((color[0] & mask) != 0 ? "x" :
					(color[1] & mask) != 0 ? "o" : ".");
			}
			strm << endl;
		}
		if (hasWon_impl())
			strm << "won" << endl;
		return strm.str();
	}
};

using Connect4Game = Connect4GameExt<>;


