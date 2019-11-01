#pragma once

#include "IComponentManager.h"
#include <glm/glm.hpp>

#include <unordered_map>

using namespace dodf;
using namespace std;
using namespace glm;

class TransformComponentMgr: public IComponentManager 
{
private :
	struct InstanceData {
		vec3* position;
		vec3* rotation;
		vec3* scale;
		//Entity *entity;             ///< The entity owning this instance.
		mat4 *local;				///< Local transform with respect to parent.
		mat4 *world;				///< World transform.
		Instance *parent;           ///< The parent instance of this instance.
		Instance *firstChild;      ///< The first child of this instance.
		Instance *nextSibling;     ///< The next sibling of this instance.
		Instance *prevSibling;     ///< The previous sibling of this instance.
	};
	InstanceData m_data;

	void transform(const glm::mat4 &parent, Instance i);
public:
	
	void add(const Entity& e) override;

	//makes a swap between the last one to the one you want to destroy
	//then i destroys the last, this way you always destroy the end point
	void destroy(const Entity& e)
	{
		/*unsigned last = m_data.n - 1;
		auto last_e = m_data.entities[last];
		auto instance = m_map[e.index()];
		m_data.position[instance.i] = m_data.position[last];
		m_data.velocity[instance.i] = m_data.velocity[last];

		m_map[last_e->index()] = instance;
		m_map.erase(e.index());

		--m_data.n;*/
	}
	void allocate(size_t size) override;
	void clean();
	void setLocal(Instance i, const glm::mat4 &m);
	//void updateWorlds();

	inline const mat4& getWorldMatrix(Instance i) const { return m_data.world[i.i]; }
	inline const mat4& getLocalMatrix(Instance i) const { return m_data.local[i.i]; }
	inline mat4& referenceToLocalMatrix(Instance i) const { return m_data.local[i.i]; }
};

