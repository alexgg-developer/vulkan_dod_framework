#pragma once

#include "GLFWApp.h"
#include "common/EntityManager.h"
#include "common/MemoryPool.h"
#include "common/MovementComponent.h"
#include "common/MovementSystem.h"

#include <glm/common.hpp>

using namespace std;

class TestApp: public GLFWApp
{
public:
	void draw();
	void run();
	void testMemory();
	void testMovementComponent();
	void update(float dt);
	void updateTestMovementComponent(float dt);

private:
	EntityManager entityManager;
	MovementComponentManager movementComponentManager;
	MovementSystem movementSystem;
	std::vector<Entity> entities;
};

