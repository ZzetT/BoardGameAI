#pragma once

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "types.h"
#include "StaticVector.h"
#include "SmartVector.h"


template<int maxMoves>
class MoveList 
{
private:
	typedef SmartVector<ExtMove, maxMoves> MovesType;
	MovesType moves;
public:
	MoveList()
	{

	}
	~MoveList()
	{

	}
	friend std::ostream & operator<<(std::ostream & out, MoveList const & moveList)
	{
		for (auto& move : moveList.moves)
		{
			out << std::to_string(move.move);
		}
		return out;
	}
	void clear() {
		moves.clear();
	}
	auto data()
	{
		return &moves;
	}
	auto data() const
	{
		return (const std::vector<ExtMove>*)&moves;
	}
	auto lastMove() const
	{
		return moves.back();
	}
	auto size() const
	{
		return moves.size();
	}
	void addMove(Move move, Value value)
	{
		moves.push_back(ExtMove(move, value));
	}
	void addMove(Move move)
	{
		moves.push_back(ExtMove(move));
	}
	void undoMove()
	{
		moves.pop_back();
	}
	
	typedef typename MovesType::iterator iterator;
	typedef typename MovesType::const_iterator const_iterator;
};
