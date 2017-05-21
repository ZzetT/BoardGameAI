#pragma once

class StatusUpdate
{
public:
	virtual void sendCurrentDepth(int depth) = 0;
	virtual void sendPrelimaryResult(unsigned int depth, int bestMove, int16_t score) = 0;
};