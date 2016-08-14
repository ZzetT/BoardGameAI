#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

class TraceOutputNode
{
public:
	TraceOutputNode* parent;
	std::string name;
	int alpha;
	int beta;
	int value;
	std::map<int, std::unique_ptr<TraceOutputNode>> childs;

	TraceOutputNode();
	TraceOutputNode(TraceOutputNode *parent_) : parent(parent_) {}
	TraceOutputNode(int value, int alpha, int beta) : value(value), alpha(alpha), beta(beta) {}
	TraceOutputNode(std::string name, TraceOutputNode *parent_ = nullptr) :name(name), parent(parent_) {}
	~TraceOutputNode();

	TraceOutputNode& operator[](const int move)
	{
		auto it = childs.find(move);
		if (it == childs.end())
		{
			childs[move] = std::make_unique<TraceOutputNode>(this);
		}
		return *childs[move].get();
	}
	void set(int value_, int alpha_, int beta_) { value = value_; alpha = alpha_; beta = beta_; }
	void set(int value_) { value = value_;}
	bool matchExact(const TraceOutputNode * traceOutputNode);
};

