#pragma once

#include "Entity.h"
#include "MemoryPool.h"
#include "Instance.h"

#include <unordered_map>
#include <vector>

using namespace dodf;
using namespace std;

class IComponentManager 
{
protected :
	struct MemoryBuffer {
		size_t n;          ///< Number of used instances.
		size_t allocated;  ///< Number of allocated instances.
		size_t memoryUsed;
		void *data;        ///< Buffer with instance data.
	};
	//references to the entities are using this component
	std::vector<Entity*> m_entities;
	//entity id, to instance
	//it is recommended to use size_t or ptrdiff_t for arrays and pointer arithmetic
	//ref: https://www.viva64.com/en/a/0050/#ID0ENIAE
	unordered_map<size_t, Instance> m_map;	
	MemoryBuffer m_buffer;
public:
	//gets/sets
	Instance lookup(const Entity& e) const { return m_map.at(static_cast<size_t>(e.index())); }
	virtual void add(const Entity& e) = 0;

	virtual void allocate(size_t size) = 0;
};

