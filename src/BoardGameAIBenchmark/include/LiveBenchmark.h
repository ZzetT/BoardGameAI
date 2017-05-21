#pragma once
#include "RecordedBenchmark.h"
#include "CSVIterator.h"

template<class AIType>
class LiveBenchmark : public RecordedBenchmark
{
private:
	typedef typename AIType::AIGameType GameType;
	bool switchedToWriteMode = false;

	std::shared_ptr<GameType> game = std::make_shared<GameType>();
	std::shared_ptr<AIType> ai;

public:
	LiveBenchmark(const std::shared_ptr<AIType>& ai, std::string directory, std::string name, bool overwrite = false) :
		RecordedBenchmark(directory, name, std::fstream::in | std::fstream::out | (overwrite ? std::fstream::trunc : std::fstream::app)),
		ai(ai)
	{
	}
	~LiveBenchmark()
	{
	}

	void runPosition(std::string const& moves, int expectedValue)
	{
		if (csvIterator != nullptr && *csvIterator != CSVIterator())
		{
			// load results from file
			RecordedBenchmark::runPosition(moves, expectedValue);
		}
		else
		{
			// write results to file
			if (!switchedToWriteMode)
			{
				cacheFile.clear();
				cacheFile.seekg(0, std::fstream::end);
				switchedToWriteMode = true;
			}
			if (!headerWritten)
			{
				cacheFile << "Moves,Expected,Actual,Nodes,Time(ms)" << std::endl;
				headerWritten = true;
			}
			game->clear();
			ai->resetAnalyzedPositionsCounter();
			for (auto move : moves)
			{
				game->makeMove(move - '0');
			}
			auto startTime = chrono::system_clock::now();
			SearchResult result = ai->search(game);
			long long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count();
			auto nodes = ai->analyzedPositions();
			addGameResult(time, nodes, expectedValue != result.value, false);
			cacheFile << moves << "," << expectedValue << "," << result.value << "," << lastNodes << "," << lastTime << std::endl;
		}
	}
};
