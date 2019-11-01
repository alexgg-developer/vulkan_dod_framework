#include "MovingTriangleApp.h"

#include <vector>
#include <assert.h>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace dodf;

void MovingTriangleApp::run()
{
	init();
	mainLoop();
	m_vulkanRenderer.cleanup();
}

void MovingTriangleApp::update(float dt)
{
	m_movementSystem.simulate(
		m_movementComponentManager.getVelocities(), 
		m_movementComponentManager.getPositions(), 
		m_movementComponentManager.getSize(), 
		dt);

	for (size_t i = 0; i < 300 ; ++i) {
		auto instance = m_transformComponentMgr.lookup(m_entities[i]);
		auto& localMatrix = m_transformComponentMgr.referenceToLocalMatrix(instance);
		localMatrix = glm::rotate(mat4(1.0), static_cast<float>(currentElapsedTime) * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		m_transformComponentMgr.setLocal(instance, localMatrix);
	}
}

void MovingTriangleApp::initVulkan()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	m_vulkanRenderer.createInstance(extensions);
	if (glfwCreateWindowSurface(m_vulkanRenderer.getInstance(), m_window, nullptr, &m_vulkanRenderer.Surface()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
	m_vulkanRenderer.init(m_meshComponentMgr);
}

void MovingTriangleApp::init()
{
	GLFWApp::init();

	//size_t initSize = 614572800;
	size_t initSize = 4014572800;
	MemoryPool::Initialize(initSize);
	m_movementComponentManager.allocate(1000);
	m_transformComponentMgr.allocate(1000);
	m_meshComponentMgr.allocate(10000);
	initVulkan();

	/*m_movementComponentManager.add(m_entities[m_entities.size() - 1]);
	m_movementComponentManager.setPosition(m_movementComponentManager.lookup(m_entities[m_entities.size() - 1]), glm::vec3(1.0f, 1.0f, 1.0f));
	m_movementComponentManager.setVelocity(m_movementComponentManager.lookup(m_entities[m_entities.size() - 1]), glm::vec3(0.1f, 0.25f, -0.1f));*/



	/*const std::vector<Vertex> m_vertices = {
		{{-0.1f, -0.1f}, {1.0f, 0.0f, 0.0f}},
		{{-0.1f, 0.1f}, {0.0f, 1.0f, 0.0f}},
		{{0.1f, -0.1f}, {0.0f, 0.0f, 1.0f}},
		{{0.1f, 0.1f}, {1.0f, 0.0f, 0.0f}}
	};*/

	for (size_t i = 0; i < 300; ++i) {
		m_entities.emplace_back(m_entityManager.create());
		m_transformComponentMgr.add(m_entities[m_entities.size() - 1]);

		float originX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float originY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float originZ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		const std::vector<Vertex> m_vertices = {	
			{{-0.1f + originX, -0.1f + originY, originZ}, {1.0f, 0.0f, 0.0f}},
			{{-0.1f + originX, 0.1f + originY, originZ}, {0.0f, 1.0f, 0.0f}},
			{{0.1f + originX, -0.1f + originY, originZ}, {0.0f, 0.0f, 1.0f}},
			{{0.1f + originX, 0.1f + originY, originZ}, {1.0f, 0.0f, 0.0f}}
		};

		const std::vector<uint32_t> m_indices = {
			0, 1, 2,
			1, 3, 2
		};

		m_meshComponentMgr.add(m_entities[m_entities.size() - 1]);
		m_meshComponentMgr.setMesh(m_meshComponentMgr.lookup(m_entities[m_entities.size() - 1]), m_vertices, m_indices);
	}
	m_vulkanRenderer.createCommandBuffers(m_meshComponentMgr);
}

void MovingTriangleApp::draw()
{
	m_vulkanRenderer.draw(b_framebufferResized, m_entities, m_transformComponentMgr, m_meshComponentMgr);
	b_framebufferResized = false;
}