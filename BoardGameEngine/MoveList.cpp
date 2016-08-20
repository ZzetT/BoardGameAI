#include "stdafx.h"
#include "MoveList.h"


MoveList::MoveList()
{
}

MoveList::~MoveList()
{
}

void MoveList::sort()
{
	std::sort(begin(), end(), [](Move a, Move b) { return a.value > b.value; });
}

std::ostream & operator<<(std::ostream & out, MoveList const & moveList)
{
	for (auto& move : moveList)
	{
		out << std::to_string(move.move);
	}
	return out;
}
