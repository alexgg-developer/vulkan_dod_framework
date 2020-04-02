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


ModelVisualizerApp::ModelVisualizerApp(): m_materialMgr(MaterialMgr::Get()), m_idMgr(IdManager::Get())
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
	m_idMgr.allocate(10000);
	m_transformComponentMgr.allocate(1000);
	m_meshComponentMgr.allocate(1000);	
	m_materialMgr.allocate(1000);
	initVulkan();	

	std::function<void(const Mesh<TexturedVertex> &) > addModelLambda = [this](const Mesh<TexturedVertex>& model) { addModel(model); };
	//ModelLoader::loadMeshes("eyeball.obj", addModelLambda);
	ModelLoader::loadMeshes("cube.obj", addModelLambda);

	m_vulkanRenderer.createCommandBuffers(m_meshComponentMgr);
}

void ModelVisualizerApp::addModel(const Mesh<TexturedVertex>& mesh)
{
	m_entities.emplace_back(m_entityManager.create());
	m_transformComponentMgr.add(m_entities[m_entities.size() - 1]);

	Instance transformInstance = m_transformComponentMgr.lookup(m_entities[m_entities.size() - 1]);

	auto& localMatrix = m_transformComponentMgr.referenceToLocalMatrix(transformInstance);
	localMatrix = glm::rotate(mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//localMatrix = glm::translate(mat4(1.0),  glm::vec3(0.0f, 0.0f, -5.0f));
	//localMatrix = glm::scale(mat4(1.0),  glm::vec3(0.2f, 0.2f, 0.2f));
	localMatrix = glm::scale(localMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
	m_transformComponentMgr.setLocal(transformInstance, localMatrix);

	m_meshComponentMgr.add(m_entities[m_entities.size() - 1]);
	m_meshComponentMgr.setMesh(m_meshComponentMgr.lookup(m_entities[m_entities.size() - 1]), mesh);
}

void ModelVisualizerApp::draw()
{
	m_vulkanRenderer.draw(b_framebufferResized, m_entities, m_transformComponentMgr, m_meshComponentMgr);
	b_framebufferResized = false;
}