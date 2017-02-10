#pragma once

#include "types.h"

/// The TTEntry is the 96 bit transposition table entry, defined as below:
///
/// key: 32 bit
/// move: 16 bit
/// bound type: 8 bit
/// generation: 8 bit
/// depth: 16 bit
/// value: 16 bit
struct TTEntry {

public:
	void save(uint32_t k, int16_t v, Bound b, uint16_t d, Move m, uint8_t g) {

		key32 = k;
		move16 = m;
		bound8 = b;
		generation8 = g;
		value16 = v;
		depth16 = d;
	}

	uint32_t key() const { return key32; }
	Move move() const { return move16; }
	Bound bound() const { return bound8; }
	int16_t value() const { return value16; }
	uint16_t depth() const { return depth16; }
	uint8_t generation() const { return generation8; }

private:
	uint32_t key32;
	Move move16;
	Bound bound8;
	uint8_t generation8;
	uint16_t depth16;
	int16_t value16;
};

static_assert (sizeof(TTEntry) == 12, "Size of TTEntry is not correct");
