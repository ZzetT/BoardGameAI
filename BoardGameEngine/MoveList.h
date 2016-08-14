#pragma once

#include <vector>
#include <string>

class MoveList :
	public std::vector<int>
{
public:
	MoveList();
	~MoveList();
	friend std::ostream & operator<<(std::ostream & Str, MoveList const & v);
};

