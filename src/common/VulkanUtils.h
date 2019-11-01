#pragma once

#include <vulkan/vulkan.h>
#include "DebugUtils.h"

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
};
