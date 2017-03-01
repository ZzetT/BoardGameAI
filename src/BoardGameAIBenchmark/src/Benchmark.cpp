#include <iostream> 
#include <iomanip>
#include <fstream>
#include <ctime>
#include "Connect4Game.h"
#include "BoardGameAI.h"
#include "BenchmarkManager.h"

using namespace std;


int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		string directory = argv[1];
		string testData = "Test_L2_R1";

		auto ai1 = AIBuilder<Connect4Game>{}.useTTable().create();
		auto ai2 = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().create();
		auto ai3 = AIBuilder<Connect4Game>{}.iterativeDeepening().usePVS().useTTable().create();

		auto benchmark1 = new RecordedBenchmark(directory, "TTAI");
		auto benchmark2 = new RecordedBenchmark(directory, "IterativeTTAI");
		auto benchmark3 = new RecordedBenchmark(directory, "IterativeTTAI PVS");

		const vector<AbstractBenchmark*> benchmarks{ benchmark1, benchmark2, benchmark3 };
		auto benchmarkManager = new BenchmarkManager(benchmarks);
		benchmarkManager->run(directory + PATH_SEPERATOR + testData);
	}
	else
	{
		std::cerr << "Please specify directory for test data and recorded benchmarks as command line argument" << std::endl;
	}
	return 0;



}
