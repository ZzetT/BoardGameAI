#pragma once

#include <string>

class Move
{
public:
	Move(int _move) : move(_move) {};
	Move(int _move, int _value) : move(_move), value(_value) {};
	~Move();

	int move;
	int value;
	friend std::ostream & operator<<(std::ostream & out, Move const & m)
	{
		out << std::to_string(m.move);
		return out;
	}
};

