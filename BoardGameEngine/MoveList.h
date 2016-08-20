#pragma once

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "Move.h"

class MoveList :
	public std::vector<Move>
{
public:
	MoveList();
	~MoveList();
	friend std::ostream & operator<<(std::ostream & Str, MoveList const & v);
	void sort();
	
};

