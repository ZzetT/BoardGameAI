#pragma once


#include <cstdint>
#include <string>

const char PATH_SEPERATOR = '/';

class AbstractBenchmark
{
private:
	uint64_t totalNodes = 0;
	unsigned int totalGames = 0;
	long long totalTime = 0LL;
	uint64_t lastNodes;
	long long lastTime;
	std::string name;
	int errors = 0;
	int cached = 0;
protected:
	void addGameResult(long long time, uint64_t nodes, bool error, bool cache)
	{
		lastTime = time;
		totalTime += time;
		lastNodes = nodes;
		totalNodes += nodes;
		if (error) errors++;
		if (cache) cached++;
		totalGames++;
	}
public:
	virtual void runPosition(std::string const& moves, int expectedValue) = 0;
	double getAverageNodes()
	{
		return (double)totalNodes / (double)totalGames;
	}
	double getAverageTime()
	{
		return (double)totalTime / (double)totalGames;
	}
	uint64_t getLastNodes()
	{
		return lastNodes;
	}
	uint64_t getLastTime()
	{
		return lastTime;
	}
	const std::string getName()
	{
		return name;
	}
	int getErrors()
	{
		return errors;
	}
	int getCached()
	{
		return cached;
	}

	AbstractBenchmark(std::string name) : name(name)
	{

	}
	virtual ~AbstractBenchmark()
	{

	}
};