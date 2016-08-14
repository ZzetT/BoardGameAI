#include "stdafx.h"
#include "TraceOutputNode.h"


TraceOutputNode::TraceOutputNode()
{
}


TraceOutputNode::~TraceOutputNode()
{
}

bool TraceOutputNode::matchExact(const TraceOutputNode * other)
{
	if (value != other->value)
	{
		return false;
	}
	if (childs.size() != other->childs.size())
	{
		return false;
	}
	for (auto& it : childs)
	{
		int move = it.first;
		auto found = other->childs.find(move);
		if (found == other->childs.end())
		{
			return false;
		}
		if (!it.second->matchExact(found->second.get()))
		{
			return false;
		}
	}
	if (!childs.empty())
	{
		if (alpha != other->alpha)
		{
			return false;
		}
		if (beta != other->beta)
		{
			return false;
		}
	}
	return true;
}
