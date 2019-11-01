#pragma once

#include "Entity.h"
#include "MemoryPool.h"
#include "Instance.h"

#include <glm/common.hpp>

using namespace dodf;
using namespace std;

class MovementSystem 
{
private :
	public:
		void simulate(glm::vec3 const * const velocity, glm::vec3 * const position, uint32_t n, float dt)
		{
			for (unsigned i = 0; i < n; ++i) {
				position[i] += velocity[i] * dt;
			}
		}
};

