#pragma once

#include <glm/common.hpp>

#include "GLFWApp.h"
#include "common/EntityManager.h"
#include "common/MemoryPool.h"
#include "common/MovementComponent.h"
#include "common/TransformComponentMgr.h"
#include "common/MeshComponentMgr.h"
#include "common/MovementSystem.h"
#include "common/VulkanRenderer.h"
#include "common\ModelLoader.h"
#include "common\MaterialMgr.h"
#include "common\IdManager.h"


using namespace std;

class ModelVisualizerApp: public GLFWApp
{
public:
	ModelVisualizerApp();
	void init() override final;
	void draw();
	void run();
	void update(float dt);
	virtual ~ModelVisualizerApp() {};
private:
	void addModel(const Mesh<TexturedVertex>& model);
	void initVulkan();

	EntityManager m_entityManager;
	std::vector<Entity> m_entities;
	TransformComponentMgr m_transformComponentMgr;
	MeshComponentMgr m_meshComponentMgr;
	IdManager& m_idMgr;
	MaterialMgr& m_materialMgr;

	VulkanRenderer m_vulkanRenderer;

};

