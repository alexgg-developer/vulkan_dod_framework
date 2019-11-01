#pragma once

#include "VulkanUtils.h"

namespace dodf {
	namespace CommandManager {
		enum class Pool
		{
			GRAPHICS,
			TRANSFER,
			COMPUTE
		};
		VkCommandBuffer beginSingleUseCmdBuffer(VkCommandBufferLevel level, Pool type, VkDevice device);
		void endSingleUseCmdBuffer(VkCommandBuffer cmdBuffer, Pool type, VkDevice device);

		VkCommandBuffer createCommandBuffer(VkCommandBufferLevel level, Pool type, VkDevice device);
		void createCommandPools(VkDevice device);
		VkCommandBufferAllocateInfo commandBufferAllocateInfo(VkCommandPool commandPool, VkCommandBufferLevel level, uint32_t bufferCount);
		void destroy(VkDevice device);
		VkCommandPool getPoolByType(Pool type);
	}
}