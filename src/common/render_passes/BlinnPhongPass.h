#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

class BlinnPhongPass
{
private:
	std::string vertexShaderPath = "shaders/compiled/shader_blinn_base_vert.spv";
	std::string fragmentShaderPath = "shaders/compiled/shader_blinn_base_frag.spv";

	std::vector<VkDescriptorSetLayoutBinding> m_bindings;
	std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
public:
	void createDescriptorSetLayout(VkDevice device);
};