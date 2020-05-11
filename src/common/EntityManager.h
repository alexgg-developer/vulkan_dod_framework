#pragma once

#include "Assert.h"
#include "Entity.h"

#include <stdint.h>
#include <deque>
#include <unordered_set>

using namespace std;

class EntityManager
{
private:
	const uint32_t MINIMUM_FREE_INDICES = 1024;

	std::vector<uint8_t> m_generation;
	std::deque<uint32_t> m_freeIndices;
public:
	EntityManager() {}
	Entity create();
	bool alive(Entity e) const;
	void destroy(Entity e);
	void reset();
};
