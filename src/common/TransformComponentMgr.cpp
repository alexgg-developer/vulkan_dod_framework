#include "TransformComponentMgr.h"
#include "Assert.h"
#include "MemoryPool.h"

#include <glm/gtc/matrix_transform.hpp>

void TransformComponentMgr::clean()
{

}


void TransformComponentMgr::allocate(size_t size)
{
	//const unsigned bytes = size * 3 * sizeof(glm::vec3) + size * 2 * sizeof(glm::mat4);
	const size_t bytes = size * 3 * sizeof(glm::vec3) + size * 2 * sizeof(glm::mat4) + size * 4 * sizeof(Instance*);
	m_buffer.data = MemoryPool::Get(bytes);
	m_buffer.allocated = size;
	m_data.position = static_cast<glm::vec3*>(m_buffer.data);
	m_data.rotation = (m_data.position + size);
	m_data.scale = (m_data.rotation + size);
	m_data.local = reinterpret_cast<mat4*>(m_data.scale + size);
	m_data.world = m_data.local + size;
	m_data.parent = reinterpret_cast<Instance*>(m_data.world + size);
	m_data.firstChild = m_data.parent + size;
	m_data.nextSibling = m_data.firstChild + size;
	m_data.prevSibling = m_data.nextSibling + size;
	m_buffer.n = 0;
}

void TransformComponentMgr::add(const Entity& e)
{
	ASSERT(m_buffer.n + 1 < m_buffer.allocated);
	auto instance = Instance::create(m_buffer.n);
	m_map[e.index()] = instance;
	m_data.position[instance.i] = glm::vec3(0.0);
	m_data.rotation[instance.i] = glm::vec3(0.0);
	m_data.scale[instance.i] = glm::vec3(1.0);
	m_data.local[instance.i] = glm::mat4(1.0);
	m_data.world[instance.i] = glm::mat4(1.0);
	//m_data.world[instance.i] = glm::rotate(m_data.world[instance.i], glm::radians(180.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	m_data.parent[instance.i] = Instance::create(Instance::NO_VALUE);
	m_data.firstChild[instance.i] = Instance::create(Instance::NO_VALUE);
	m_data.nextSibling[instance.i] = Instance::create(Instance::NO_VALUE);
	m_data.prevSibling[instance.i] = Instance::create(Instance::NO_VALUE);
	++m_buffer.n;
}

//TO DO: could be improved you create a sorted vector with the data and you iterate in order of parent - childs like a tree traversal
void TransformComponentMgr::setLocal(Instance i, const glm::mat4 &m)
{
	m_data.local[i.i] = m;
	Instance parent = m_data.parent[i.i];
	glm::mat4 parentModel = parent.isValid() ? m_data.world[parent.i] : glm::mat4(1.0f);
	transform(parentModel, i);
}

/*void TransformComponentMgr::updateWorlds()
{
}*/

void TransformComponentMgr::transform(const glm::mat4 &parent, Instance i)
{
	m_data.world[i.i] = m_data.local[i.i] * parent;

	Instance child = m_data.firstChild[i.i];
	while (child.isValid()) {
		transform(m_data.world[i.i], child);
		child = m_data.nextSibling[child.i];
	}
}