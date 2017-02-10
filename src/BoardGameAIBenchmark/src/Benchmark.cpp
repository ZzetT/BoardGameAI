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
	string testData = "Test_L2_R1";

	auto ai1 = AIBuilder<Connect4Game>{}.useTTable().create();
	auto ai2 = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().create();

	auto benchmark1 = new RecordedBenchmark("TTAI");
	auto benchmark2 = new RecordedBenchmark("IterativeTTAI");
	auto benchmark3 = new RecordedBenchmark("IterativeTTAI static eval");
	auto benchmark4 = new LiveBenchmark<Connect4Game, decltype(ai2)>(std::make_shared<decltype(ai2)>(), "IterativeTTAI static eval2");

	const vector<AbstractBenchmark*> benchmarks{ benchmark1, benchmark2, benchmark3, benchmark4 };
	auto benchmarkManager = new BenchmarkManager(benchmarks);
	benchmarkManager->run(testData);
	return 0;



}
