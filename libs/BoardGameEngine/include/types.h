#pragma once

#include <cstdint>

enum Bound : uint8_t {
	BOUND_NONE,
	BOUND_UPPER,
	BOUND_LOWER,
	BOUND_EXACT = BOUND_UPPER | BOUND_LOWER
};

const int MAX_PLY = 128;

static const int WINNING_VALUE = 32000;

static const int WINNING_IN_MAX_PLY = WINNING_VALUE - MAX_PLY;

static const int INFINITE = 32700;

typedef uint16_t Value;

typedef uint16_t Move;

class ExtMove {
public:
	Move move;
	Value value;

	ExtMove(Move move, Value value) : move(move), value(value) {}
	ExtMove(Move move) : move(move) {}
	ExtMove() {}
	operator Move() const { return move; }
	void operator=(Move m) { move = m; }
};

inline bool operator<(const ExtMove& f, const ExtMove& s) {
	return f.value < s.value;
}

namespace AIOptions
{
	enum {
		None =				1 << 0,
		DoTrace =			1 << 1, // trace steps during search
		Iterate =			1 << 2, // iterative deepening
		Evaluate =			1 << 3, // use evaluation function of game
		HistoryHeuristic =  1 << 4, // use history heuristics
		TimeManagement =	1 << 5 // cancel search after time is over
	};
}

#define ITERATE_ENABLED(o) ((o & AIOptions::Iterate) != 0)
#define HISTORY_ENABLED(o) ((o & AIOptions::HistoryHeuristic) != 0)
#define TIMER_ENABLED(o) ((o & AIOptions::TimeManagement) != 0)
