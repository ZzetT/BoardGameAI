#pragma once
#include "Trace.h"
#include "TraceOutputNode.h"
#include <vector>
#include <map>
#include <memory>

class TraceOutput :
	public Trace
{
private:
	std::map<unsigned int, std::unique_ptr<TraceOutputNode>> treeByDepth;
	unsigned int currentDepth;
	TraceOutputNode *currentNode;
	std::shared_ptr<AbstractBoardGame> currentGame;


public:
	TraceOutput();
	~TraceOutput();

	// Inherited via Trace
	virtual void makeMove(int move) override;
	virtual void undoMove() override;
	virtual void startAlphaBetaSearch(unsigned int depth) override;
	virtual void tracePositionValue(int value) override;
	virtual void startIterativeSearch(const std::shared_ptr<AbstractBoardGame>& game) override { this->currentGame = game; }
	virtual void alphaBetaWindow(int alpha, int beta) override;
	friend std::ostream& operator<<(std::ostream&, const TraceOutput&);

	bool matchExact(int depth, const TraceOutputNode* traceOutputNode);



};

