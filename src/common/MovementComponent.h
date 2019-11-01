#pragma once

#include "Entity.h"
#include "MemoryPool.h"
#include "Instance.h"

#include <glm/common.hpp>

#include <unordered_map>

using namespace dodf;
using namespace std;

class MovementComponentManager 
{
private :
	struct InstanceData {
		unsigned n;          ///< Number of used instances.
		unsigned allocated;  ///< Number of allocated instances.
		void *buffer;        ///< Buffer with instance data.

		glm::vec3 *position;
		glm::vec3 *velocity;
		//only references
		std::vector<Entity*> entities;
	};
	InstanceData _data;
	//entity id, to instance
	//it is recommended to use size_t or ptrdiff_t for arrays and pointer arithmetic
	//ref: https://www.viva64.com/en/a/0050/#ID0ENIAE
	unordered_map<size_t, Instance> m_map;
	//allocate sz new instances in a new buffer and copy the old ones
public:
	//gets/sets
	Instance lookup(const Entity& e) { return m_map[e.index()]; }
	glm::vec3 getPosition(const Instance& i) const { return _data.position[i.i]; }
	glm::vec3* getPositions() { return _data.position; }
	void setPosition(const Instance& i, const glm::vec3& position ) { _data.position[i.i] = position; }
	void addPosition(const Instance& i, const glm::vec3& position ) { _data.position[i.i] += position; }
	glm::vec3 getVelocity(const Instance& i) const { return _data.velocity[i.i]; }	
	glm::vec3* getVelocities() { return _data.velocity; }
	void setVelocity(const Instance& i, const glm::vec3& velocity) { _data.velocity[i.i] = velocity; }
	unsigned getSize() { return _data.n; }
	
	void add(const Entity& e)
	{
		assert(_data.n + 1 < _data.allocated);
		auto instance = Instance::create(_data.n);
		m_map[e.index()] = instance;
		_data.position[instance.i] = glm::vec3(0.0);
		_data.velocity[instance.i] = glm::vec3(0.0);
		++_data.n;
	}

	//makes a swap between the last one to the one you want to destroy
	//then i destroys the last, this way you always destroy the end point
	void destroy(const Entity& e)
	{
		size_t last = _data.n - 1;
		auto last_e = _data.entities[last];
		auto instance = m_map[e.index()];
		_data.position[instance.i] = _data.position[last];
		_data.velocity[instance.i] = _data.velocity[last];

		m_map[last_e->index()] = instance;
		m_map.erase(e.index());

		--_data.n;
	}
	
	void allocate(uint32_t sz)
	{
		//assert(sz > _data.n);

		//InstanceData new_data;
		//const unsigned bytes = sz * (sizeof(Entity) + sizeof(float) + 2 * sizeof(glm::vec3));
		//const unsigned bytes = sz * (sizeof(float) + 2 * sizeof(glm::vec3));
		const size_t bytes = sz * 2 * sizeof(glm::vec3);
		//_data.buffer = reinterpret_cast<int*>(MemoryPool::Get(bytes));
		_data.buffer = static_cast<int*>(MemoryPool::Get(bytes));
		_data.allocated = sz;
		_data.position = static_cast<glm::vec3*>(_data.buffer);
		_data.velocity = (_data.position + sz);
		_data.n = 0;
	}
};

