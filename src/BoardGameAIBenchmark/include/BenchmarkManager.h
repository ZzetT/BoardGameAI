#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>
#include "LiveBenchmark.h"

class BenchmarkManager
{
private:
	std::vector<AbstractBenchmark*> benchmarks;
	const char emptySpace = ' ';
	const int refreshTimeMs = 5000;
	size_t maximumNameLength = 0;
public:
	BenchmarkManager(const std::vector<AbstractBenchmark*>& benchmarks) : benchmarks(benchmarks) {
		for (auto benchmark : benchmarks)
		{
			maximumNameLength = std::max(maximumNameLength, benchmark->name.length());
		}
	}
	//split function taken from http://stackoverflow.com/a/236803/5107610
	void split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
	}


	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}
	void run(std::string testDataFilename)
	{
		unsigned int position = 0;
		std::ifstream testDataFile;
		testDataFile.open(testDataFilename, std::ios::in);
		testDataFile.unsetf(std::ios_base::skipws);
		auto totalLines = std::count(
			std::istream_iterator<char>(testDataFile),
			std::istream_iterator<char>(),
			'\n');
		testDataFile.clear();
		testDataFile.seekg(0, std::ios::beg);
		std::cout << std::setprecision(2) << std::showpoint << std::fixed;

		auto startTime = std::chrono::system_clock::now() - std::chrono::milliseconds(refreshTimeMs);

		std::string s;
		while (getline(testDataFile, s))
		{
			position++;
			if (s.empty())
			{
				continue;
			}

			auto lastRefreshMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count();
			bool refresh;
			if (lastRefreshMs >= refreshTimeMs || position == totalLines)
			{
				startTime = std::chrono::system_clock::now();
				std::cout << "Progress: " << position << "/" << totalLines << " " << std::endl;
				refresh = true;
			}
			else
			{
				refresh = false;
			}

			for (auto benchmark : benchmarks)
			{
				std::vector<std::string> x = split(s, ' ');
				std::string moves = x[0];
				int expectedValue = stoi(x[1]);
				benchmark->runPosition(moves, expectedValue);
				if (refresh)
				{
					std::ostringstream nodesText;
					nodesText << std::setprecision(2) << std::showpoint << std::fixed << benchmark->averageNodes() << " nodes ";
					std::ostringstream durationText;
					durationText << std::setprecision(2) << std::showpoint << std::fixed << benchmark->averageTime() << " ms ";
					std::cout << std::left << std::setw(maximumNameLength + 1) << std::setfill(emptySpace) << benchmark->name
						<< std::setw(20) << nodesText.str()
						<< std::setw(20) << durationText.str()
						<< "errors " << benchmark->errors
						<< " cached " << benchmark->cached
						<< std::endl;
				}
			}
			if (refresh) std::cout << std::endl;
		}
		testDataFile.close();
	}
};
