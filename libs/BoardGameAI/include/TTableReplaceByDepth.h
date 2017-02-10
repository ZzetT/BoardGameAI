#pragma once

#include "TTEntry.h"
#include <memory>
#include <array>

template<int mbSize>
class TTableReplaceByDepth
{
private:
	static const size_t elements = (mbSize == 0 ? 1 /* dummy element */ : mbSize * 1024 * 1024 / sizeof(TTEntry));
	std::array<TTEntry, elements> table;
	int generation = 0;
public:
	TTableReplaceByDepth() {};
	~TTableReplaceByDepth() {};
	void new_search() { generation++; }
	const TTEntry* probe(const uint64_t key) const;
	void store(const uint64_t key, int16_t v, Bound type, uint16_t d, Move m);
};


template<int mbSize>
const TTEntry * TTableReplaceByDepth<mbSize>::probe(const uint64_t key) const
{
	const TTEntry* tte = &table[static_cast<uint32_t>(key) % elements];
	if (tte->key() == key >> 32)
	{
		return tte;
	}

	return nullptr;
}

template<int mbSize>
void TTableReplaceByDepth<mbSize>::store(const uint64_t key, int16_t v, Bound type, uint16_t d, Move m)
{
	TTEntry *entry = &table[static_cast<uint32_t>(key) % elements];
	if (entry->generation() != generation || entry->depth() < d)
	{
		entry->save(key >> 32, v, type, d, m, 0);
	}
}