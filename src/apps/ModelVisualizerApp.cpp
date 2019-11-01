#include "ModelVisualizerApp.h"

#include <vector>
#include <assert.h>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace dodf;

void ModelVisualizerApp::run()
{
	init();
	mainLoop();
	m_vulkanRenderer.cleanup();
}

void ModelVisualizerApp::update(float dt)
{	

}

void ModelVisualizerApp::initVulkan()
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

void ModelVisualizerApp::init()
{
	GLFWApp::init();

	//size_t initSize = 614572800;
	size_t initSize = 4014572800;
	MemoryPool::Initialize(initSize);
	m_transformComponentMgr.allocate(1000);
	m_meshComponentMgr.allocate(1000);
	initVulkan();	
}

void ModelVisualizerApp::draw()
{
	m_vulkanRenderer.draw(b_framebufferResized, m_entities, m_transformComponentMgr, m_meshComponentMgr);
	b_framebufferResized = false;
}