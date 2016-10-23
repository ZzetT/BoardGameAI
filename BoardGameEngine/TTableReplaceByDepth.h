#pragma once

#include "TTEntry.h"
#include <memory>
#include <array>

template<int mbSize>
class TTableReplaceByDepth
{
private:
	std::array<TTEntry, mbSize * 1024 * 1024 / sizeof(TTEntry)> table;
	const size_t elements = table.size();
public:
	TTableReplaceByDepth() {};
	~TTableReplaceByDepth() {};
	void new_search() { }
	const TTEntry* probe(const uint64_t key) const;
	void store(const uint64_t key, int16_t v, Bound type, uint16_t d, Move m);
};


template<int mbSize>
const TTEntry * TTableReplaceByDepth<mbSize>::probe(const uint64_t key) const
{
	const TTEntry* tte = &table[(uint32_t)key % elements];
	uint32_t key32 = key >> 32;

	if (tte->key32 == key32)
	{
		return tte;
	}

	return nullptr;
}

template<int mbSize>
void TTableReplaceByDepth<mbSize>::store(const uint64_t key, int16_t v, Bound type, uint16_t d, Move m)
{
	TTEntry *entry = &table[(uint32_t)key % elements];
	uint32_t key32 = key >> 32;
	if (entry->key32 != key32 || entry->key32 == key32 && entry->depth16 < d)
	{
		entry->save(key32, v, type, d, m, 0);
	}
}