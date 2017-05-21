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
		string testData = "Test_L2_R2";

		auto ai1 = AIBuilder<Connect4GameExt<false>>{}.iterativeDeepening().useTTable().useMateDistancePruning().create();
		auto ai2 = AIBuilder<Connect4GameExt<true, false>>{}.iterativeDeepening().useTTable().useMateDistancePruning().create();
		auto ai3 = AIBuilder<Connect4GameExt<true, true>>{}.iterativeDeepening().useTTable().useMateDistancePruning().create();
		auto ai4 = AIBuilder<Connect4Game>{}.iterativeDeepening().useTTable().useMateDistancePruning().usePVS().create();
		auto ai5 = AIBuilder<Connect4GameExt<true, true>>{}.iterativeDeepening().useTTable().useMateDistancePruning().useCouterMoveHeuristic().create();

		auto benchmark1 = new LiveBenchmark<decltype(ai1)>(std::make_shared<decltype(ai1)>(), directory, "L2 R2 IT TT MDP movesOrdered");
		auto benchmark2 = new LiveBenchmark<decltype(ai2)>(std::make_shared<decltype(ai2)>(), directory, "L2 R2 IT TT MDP staticEval dynArr");
		auto benchmark3 = new LiveBenchmark<decltype(ai3)>(std::make_shared<decltype(ai3)>(), directory, "L2 R2 IT TT MDP staticEval statArr");
		auto benchmark4 = new LiveBenchmark<decltype(ai4)>(std::make_shared<decltype(ai4)>(), directory, "L2 R2 IT TT MDP movesOrdered PVS");
		auto benchmark5 = new LiveBenchmark<decltype(ai5)>(std::make_shared<decltype(ai5)>(), directory, "L2 R2 IT TT MDP staticEval statArr HH");
		auto benchmark6 = new LiveBenchmark<decltype(ai5)>(std::make_shared<decltype(ai5)>(), directory, "L2 R2 IT TT MDP staticEval statArr CMH");

		const vector<AbstractBenchmark*> benchmarks{ benchmark1, benchmark2, benchmark3, benchmark4, benchmark5, benchmark6 };
		auto benchmarkManager = new BenchmarkManager(benchmarks);
		benchmarkManager->run(directory + PATH_SEPERATOR + testData);
	}
	else
	{
		std::cerr << "Please specify directory for test data and recorded benchmarks as command line argument" << std::endl;
	}
	return 0;



}
