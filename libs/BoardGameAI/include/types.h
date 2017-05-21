#pragma once

#include <cstdint>

enum Bound : uint8_t {
	BOUND_NONE,
	BOUND_UPPER,
	BOUND_LOWER,
	BOUND_EXACT = BOUND_UPPER | BOUND_LOWER
};
static const int WINNING_VALUE = 32000;

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
	void operator+=(int v) { value += v; }
};

inline bool operator<(const ExtMove& f, const ExtMove& s) {
	return f.value < s.value;
}

namespace AIOptions
{
	enum {
		None =					1 << 0,
		Iterate =				1 << 2, // iterative deepening
		CounterMove =			1 << 3, // use counter moves heuristic
		TimeManagement =		1 << 4, // cancel search after time is over
		PVS =					1 << 5, // principial variation search
		MateDistancePruning =	1 << 6, // mate distance pruning
		UpdateStatus =			1 << 7 // update status
	};
}

#define ITERATE_ENABLED(o) ((o & AIOptions::Iterate) != 0)
#define COUNTERMOVE_ENABLED(o) ((o & AIOptions::CounterMove) != 0)
#define PVS_ENABLED(o) ((o & AIOptions::PVS) != 0)
#define TIMER_ENABLED(o) ((o & AIOptions::TimeManagement) != 0)
#define MATE_DISTANCE_PRUNING_ENABLED(o) ((o & AIOptions::MateDistancePruning) != 0)
#define UPDATE_STATUS_ENABLED(o) ((o & AIOptions::UpdateStatus) != 0)

//define IF_CONSTEXPR_AVAILABLE if "constexpr if" feature from C++17 is available
#ifdef IF_CONSTEXPR_AVAILABLE
#define STATIC_IF(x) if constexpr(x)
#else
#define STATIC_IF(x) if (x) /* if x is a template parameter, the compiler should optimize at compile time */
#endif
