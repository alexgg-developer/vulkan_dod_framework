#pragma once

#include <vulkan/vulkan.h>
#include "DebugUtils.h"

#include <spirv_reflect.h>

// Macro to check and display Vulkan return results
#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		ASSERT(res == VK_SUCCESS);																		\
	}																									\
}	

namespace dodf::vkUtils
{	
	//static void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage & image, VkDeviceMemory & imageMemory);

	void allocateMemory(
		VkMemoryPropertyFlags memoryPropertyFlags,
		VkBuffer * buffer,
		VkDeviceMemory * memory,
		VkDevice device,
		VkPhysicalDevice physicalDevice);

	void createBuffer(
		VkBufferUsageFlags usageFlags,
		VkDeviceSize size,
		VkBuffer * buffer,
		VkDevice device);

	uint32_t findMemoryType(
		uint32_t typeFilter,
		VkMemoryPropertyFlags properties,
		VkPhysicalDevice physicalDevice);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkDevice device);

	
	std::vector<std::vector<VkDescriptorSetLayoutBinding>> reflectDescriptors(
		const std::string & shaderFilePath, 
		const VkShaderStageFlags & shaderStageFlags);
};
