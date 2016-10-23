#include "stdafx.h"
#include "TraceOutput.h"
#include <sstream>


TraceOutput::TraceOutput()
{
}


TraceOutput::~TraceOutput()
{
}

void TraceOutput::makeMove(int move)
{
	std::stringstream ss;
	ss << *currentGame->getMoveHistory();
	std::string name = ss.str();
	currentNode = currentNode->addChild(move, std::make_unique<TraceOutputNode>(name, currentNode));

}

void TraceOutput::undoMove()
{
	currentNode = currentNode->parent;
}

void TraceOutput::startAlphaBetaSearch(unsigned int depth)
{
	currentNode = (treeByDepth[depth] = std::make_unique<TraceOutputNode>("(root)")).get();
	currentDepth = depth;
}

void TraceOutput::tracePositionValue(int value)
{
	currentNode->value = value;
}

void TraceOutput::alphaBetaWindow(int alpha, int beta)
{
	currentNode->alpha = alpha;
	currentNode->beta = beta;
}

bool TraceOutput::matchExact(int depth, const TraceOutputNode * traceOutputNode)
{
	return treeByDepth[depth]->matchExact(traceOutputNode);
}

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

std::ostream & operator<<(std::ostream &out, const TraceOutput &trace)
{
	for (unsigned int depth = 1; depth <= trace.treeByDepth.size(); depth++)
	{
		out << "depth = " << std::to_string(depth) << std::endl;
		TraceOutputNode *entry = trace.treeByDepth.at(depth).get();
		printNode(out, entry, 0);
	}
	return out;
}


