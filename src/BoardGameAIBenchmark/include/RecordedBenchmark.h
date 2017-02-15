#include <iostream> 
#include <fstream>
#include <memory>
#include "AbstractBenchmark.h"
#include "CSVIterator.h"

class RecordedBenchmark : public AbstractBenchmark
{
protected:
	bool headerWritten = false;

	std::fstream cacheFile;
	std::unique_ptr<CSVIterator> csvIterator;


public:
	RecordedBenchmark(std::string directory, std::string name, int mode = std::fstream::in) : AbstractBenchmark(name)
	{
		const std::string filename = directory + PATH_SEPERATOR + name + ".csv";
		cacheFile.open(filename, mode);
		if (cacheFile.is_open())
		{
			csvIterator = std::make_unique<CSVIterator>(cacheFile);
			if ((*csvIterator)++ != CSVIterator())
			{
				headerWritten = true;
			}
		}
		else
		{
			std::cerr << "Couldn't open file " << filename << std::endl;
		}

	}
	~RecordedBenchmark()
	{
		cacheFile.close();
	}

	void runPosition(std::string const& moves, int expectedValue)
	{
		if (csvIterator != nullptr && *csvIterator != CSVIterator())
		{
			// load results from file
			std::size_t size = (**csvIterator).size();
			if (size == 5)
			{
				std::string movesFromFile = (**csvIterator)[0];
				if (moves.compare(movesFromFile) != 0)
				{
					std::cerr << "Exptected moves " << moves << " but found moves " << movesFromFile;
				}
				int lastStoneValue = stoi((**csvIterator)[2]);
				uint64_t nodes = stoll((**csvIterator)[3]);
				long long time = stoll((**csvIterator)[4]);
				addGameResult(time, nodes, expectedValue != lastStoneValue, true);
				++*csvIterator;
			}
			else
			{
				std::cerr << "Wrong number of elements per line expected 5 but found " << (**csvIterator).size() << std::endl;
			}
		}
	}
};