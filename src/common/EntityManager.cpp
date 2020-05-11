#include "EntityManager.h"

Entity EntityManager::create()
{
	uint32_t idx;
	if (m_freeIndices.size() > MINIMUM_FREE_INDICES) {
		idx = m_freeIndices.front();
		m_freeIndices.pop_front();
	}
	else {
		idx = static_cast<unsigned int>(m_generation.size());
		m_generation.push_back(0);
		ASSERT(idx < (1 << ENTITY_INDEX_BITS));
	}
	return Entity(idx, m_generation[idx]);
}

bool EntityManager::alive(Entity e) const
{
	return m_generation[e.index()] == e.generation();
}

void EntityManager::destroy(Entity e)
{
	const auto idx = e.index();
	++m_generation[idx];
	m_freeIndices.push_back(idx);
}

void EntityManager::reset()
{
	m_generation.clear();
	m_freeIndices.clear();
}
