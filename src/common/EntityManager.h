#pragma once

#include "Entity.h"

#include <stdint.h>
#include <assert.h>
#include <deque>
#include <unordered_set>

using namespace std;

class EntityManager
{
private:
	const uint32_t MINIMUM_FREE_INDICES = 1024;

	std::vector<uint8_t> _generation;
	std::deque<uint32_t> _free_indices;
public:
	EntityManager(): _free_indices(MINIMUM_FREE_INDICES, 0)
	{
		
	}
	Entity create()
	{
		uint32_t idx;
		if (_free_indices.size() > MINIMUM_FREE_INDICES) {
			idx = _free_indices.front();
			_free_indices.pop_front();
		}
		else {
			_generation.push_back(0);
			idx = static_cast<unsigned int>(_generation.size()) - 1;
			assert(idx < (1 << ENTITY_INDEX_BITS));
		}
		return Entity(idx, _generation[idx]);
	}

	bool alive(Entity e) const
	{
		return _generation[e.index()] == e.generation();
	}

	void destroy(Entity e)
	{
		const auto idx = e.index();
		++_generation[idx];
		_free_indices.push_back(idx);
	}
};
