#pragma once

#include "GLFWApp.h"
#include "common/EntityManager.h"
#include "common/MemoryPool.h"
#include "common/MovementComponent.h"
#include "common/TransformComponentMgr.h"
#include "common/MeshComponentMgr.h"
#include "common/MovementSystem.h"
#include "common/VulkanRenderer.h"

#include <glm/common.hpp>


using namespace std;

class ModelVisualizerApp: public GLFWApp
{
public:
	void init() override final;
	void draw();
	void run();
	void update(float dt);

private:
	void initVulkan();
	EntityManager m_entityManager;
	std::vector<Entity> m_entities;
	TransformComponentMgr m_transformComponentMgr;
	MeshComponentMgr m_meshComponentMgr;

	VulkanRenderer m_vulkanRenderer;
};

