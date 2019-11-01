#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class FileUtils
{
public: 
	
	static std::vector<char> readFile(const std::string& filename);

};
