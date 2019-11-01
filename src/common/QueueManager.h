#pragma once

#include "VulkanUtils.h"
#include <vector>

namespace dodf {
	namespace QueueManager {
		enum class Queue
		{
			GRAPHICS,
			TRANSFER,
			COMPUTE,
			PRESENT
		};

		void retrieveQueues(VkPhysicalDevice physicalDevice, VkDevice device);
		void initialize(VkPhysicalDevice physicalDevice);

		uint32_t getIndexQueue(Queue type);
		size_t getNumOfQueues();

		VkQueue getGraphicsQueue();
		VkQueue getTransferQueue();
		VkQueue getComputeQueue();
		VkQueue getPresentQueue();

		const std::vector<VkDeviceQueueCreateInfo>& getQueueCreateInfo();
		bool checkQueuesDevice(VkPhysicalDevice device, VkSurfaceKHR surface);
	}
}