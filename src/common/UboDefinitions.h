#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm/gtx/hash.hpp>

#include <array>
//#include <unordered_map>

namespace Ubo {

	struct MVP {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

}