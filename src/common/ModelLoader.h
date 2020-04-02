#pragma once

#include "VertexDefinitions.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

class ModelLoader
{
public:
	static const glm::vec3 DEFAULT_COLOR;

	using texturedModel_t = std::pair<std::vector<TexturedVertex>, std::vector<uint32_t>>;
	using model_t = std::pair<std::vector<Vertex>, std::vector<uint32_t>>;

	static texturedModel_t loadModel(const std::string& modelName);	
	static model_t loadModel(const std::string& modelName, const glm::vec3& defaultColor);

	static void loadMeshes(const std::string & modelName, std::function<void(const Mesh<TexturedVertex>&)> loadMeshCallback);

};
