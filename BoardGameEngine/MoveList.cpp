#include "stdafx.h"
#include "MoveList.h"


MoveList::MoveList()
{
}

MoveList::~MoveList()
{
}

std::ostream & operator<<(std::ostream & out, MoveList const & moveList)
{
	for (auto& move : moveList.moves)
	{
		out << std::to_string(move.move);
	}
	return out;
}
