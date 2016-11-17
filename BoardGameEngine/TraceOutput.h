#pragma once
#include "Trace.h"
#include "TraceOutputNode.h"
#include <vector>
#include <map>
#include <memory>

template<class GameType>
class TraceOutput :
	public Trace<GameType>
{
private:
	std::map<unsigned int, std::unique_ptr<TraceOutputNode>> treeByDepth;
	unsigned int currentDepth;
	TraceOutputNode *currentNode;
	std::shared_ptr<BoardGame<GameType>> currentGame;
	void printNode(std::ostream &out, TraceOutputNode *entry, int indent)
	{
		out << std::string(indent, ' ')
			<< entry->name << " [" << std::to_string(entry->alpha) << ", "
			<< std::to_string(entry->value) << ", "
			<< std::to_string(entry->beta) << "]"
			<< std::endl;
		for (auto& move : entry->moves)
		{
			printNode(out, entry->getChild(move), indent + 2);
		}
	}

public:
	TraceOutput();
	~TraceOutput();

	// Inherited via Trace
	virtual void makeMove(int move) override
	{
		std::stringstream ss;
		ss << *currentGame->getMoveHistory();
		std::string name = ss.str();
		currentNode = currentNode->addChild(move, std::make_unique<TraceOutputNode>(name, currentNode));
	}
	virtual void undoMove() override
	{
		currentNode = currentNode->parent;
	}
	virtual void startAlphaBetaSearch(unsigned int depth) override
	{
		currentNode = (treeByDepth[depth] = std::make_unique<TraceOutputNode>("(root)")).get();
		currentDepth = depth;
	}
	virtual void tracePositionValue(int value) override
	{
		currentNode->value = value;
	}
	virtual void startSearch(const std::shared_ptr<BoardGame<GameType>>& game) override
	{
		this->currentGame = game;
	}
	virtual void alphaBetaWindow(int alpha, int beta) override
	{
		currentNode->alpha = alpha;
		currentNode->beta = beta;
	}
	friend std::ostream& operator<<(std::ostream&, const TraceOutput&)
	{
		for (unsigned int depth = 1; depth <= trace.treeByDepth.size(); depth++)
		{
			out << "depth = " << std::to_string(depth) << std::endl;
			TraceOutputNode *entry = trace.treeByDepth.at(depth).get();
			printNode(out, entry, 0);
		}
		return out;
	}

	bool matchExact(int depth, const TraceOutputNode* traceOutputNode)
	{
		return treeByDepth[depth]->matchExact(traceOutputNode);
	}



};

