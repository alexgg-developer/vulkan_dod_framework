#include "BlinnPhongPass.h"

#include "..\FileUtils.h"
#include "..\VulkanUtils.h"

#include <spirv_reflect.h>


void BlinnPhongPass::createDescriptorSetLayout(VkDevice device)
{
	//auto vertShaderCode = FileUtils::readFile(vertexShaderPath);
	//auto fragShaderCode = FileUtils::readFile(fragmentShaderPath);

	/*VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.pImmutableSamplers = nullptr;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	//for textures
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	m_bindings = { uboLayoutBinding , samplerLayoutBinding };
	//std::vector<VkDescriptorSetLayoutBinding> bindings = { uboLayoutBinding };
	/*std::vector<VkDescriptorSetLayoutBinding> bindings = { uboLayoutBinding , samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();*/

	auto vertexDescriptorSets = dodf::vkUtils::reflectDescriptors(vertexShaderPath, VK_SHADER_STAGE_VERTEX_BIT);
	auto fragmentDescriptorSets = dodf::vkUtils::reflectDescriptors(fragmentShaderPath, VK_SHADER_STAGE_FRAGMENT_BIT);
	
	auto descriptorSets(vertexDescriptorSets);
	descriptorSets.insert(descriptorSets.end(), fragmentDescriptorSets.begin(), fragmentDescriptorSets.end());

	for (size_t i = 0; i < descriptorSets.size(); ++i) {
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(descriptorSets[i].size());
		layoutInfo.pBindings = descriptorSets[i].data();

		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout));
		m_descriptorSetLayouts.push_back(descriptorSetLayout);
	}
}