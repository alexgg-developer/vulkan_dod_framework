#include "QueueManager.h"

#include <vector>
#include <unordered_map>

#include "DebugUtils.h"

namespace dodf {
	namespace QueueManager {
		namespace impl {
			VkQueue m_graphicsQueue;
			VkQueue m_transferQueue;
			VkQueue m_computeQueue;
			VkQueue m_presentQueue;
			std::unordered_map<Queue, uint32_t> m_queueIndexes;
			std::vector<VkDeviceQueueCreateInfo> m_queueCreateInfo;

			const float m_queuePriorities[2] = { 1.0f, 0.0f };

			uint32_t getQueueFamilyIndex(VkQueueFlagBits queueFlags, std::vector<VkQueueFamilyProperties> queueFamilyProperties);
		}
	}
}

using namespace dodf::QueueManager::impl;

void dodf::QueueManager::initialize(VkPhysicalDevice physicalDevice)
{
	uint32_t queueCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, NULL);
	ASSERT(queueCount >= 1);
	//numTypeQueues = queueCount;// = 1;

	std::vector<VkQueueFamilyProperties> queueProps;
	queueProps.resize(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueProps.data());

	m_queueIndexes[Queue::GRAPHICS] = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT, queueProps);
	m_queueIndexes[Queue::TRANSFER] = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, queueProps);
	m_queueIndexes[Queue::COMPUTE]  = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT, queueProps);

	ASSERT(m_queueIndexes[Queue::GRAPHICS] < queueCount);

	//VkDeviceQueueCreateInfo * m_queueCreateInfo = (VkDeviceQueueCreateInfo *)malloc(sizeof(VkDeviceQueueCreateInfo) * numTypeQueues);
	m_queueCreateInfo.resize(queueCount);
	for (unsigned int i = 0; i < queueCount; i++) {
		//m_queueCreateInfo.emplace_back({});
		m_queueCreateInfo[i] = {};
		m_queueCreateInfo[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		m_queueCreateInfo[i].queueFamilyIndex = m_queueIndexes[static_cast<Queue>(i)];

		unsigned int numQueues = 1;
		// Ask for 2 GRAPHICS queues
		if (m_queueIndexes[Queue::GRAPHICS] == i && queueProps[i].queueCount > 1) {
			numQueues = 2;
		}

		m_queueCreateInfo[i].queueCount = numQueues;
		m_queueCreateInfo[i].pQueuePriorities = numQueues == 2 ? m_queuePriorities : &m_queuePriorities[1];
		m_queueCreateInfo[i].flags = 0;
	}
	//VK_CHECK_RESULT(CreateDevice(m_queueCreateInfo));
	//free(m_queueCreateInfo);
}

void dodf::QueueManager::retrieveQueues(VkPhysicalDevice physicalDevice, VkDevice device)
{
	std::vector<VkQueueFamilyProperties> queueProps;
	queueProps.resize(m_queueCreateInfo.size());
	uint32_t numTypeQueues = static_cast<uint32_t>(m_queueCreateInfo.size());
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numTypeQueues, queueProps.data());
	std::vector<VkQueue> queues(m_queueCreateInfo.size());
	for (uint32_t i = 0; i < numTypeQueues; i++) {
		vkGetDeviceQueue(device, m_queueIndexes[static_cast<Queue>(i)], 0, &queues[i]);
	}
	// Check if there was a secondary GRAPHICS queue
	if (queueProps[0].queueCount > 1) {
		vkGetDeviceQueue(device, m_queueIndexes[Queue::GRAPHICS], 1, &m_presentQueue);
	}
	else {
		m_presentQueue = queues[0];
	}

	m_graphicsQueue = queues[0];
	m_transferQueue = numTypeQueues > 1 ? queues[1] : m_graphicsQueue;
	m_computeQueue = numTypeQueues > 2 ? queues[2] : m_graphicsQueue;
}

uint32_t dodf::QueueManager::impl::getQueueFamilyIndex(VkQueueFlagBits queueFlags, std::vector<VkQueueFamilyProperties> queueFamilyProperties) 
{
	// Dedicated queue for compute
	// Try to find a queue family index that supports compute but not graphics
	if (queueFlags & VK_QUEUE_COMPUTE_BIT) {
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {
			if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
				return i;
			}
		}
	}

	// Dedicated queue for transfer
	// Try to find a queue family index that supports transfer but not graphics and compute
	if (queueFlags & VK_QUEUE_TRANSFER_BIT) {
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {
			if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
				// && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)
				&& (queueFamilyProperties[i].minImageTransferGranularity.width <= 4)) {
				return i;
			}
		}
	}

	// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
	for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {
		if (queueFamilyProperties[i].queueFlags & queueFlags) {
			return i;
		}
	}

	return UINT32_MAX;
}

uint32_t dodf::QueueManager::getIndexQueue(Queue type)
{
	return m_queueIndexes[type];
}

size_t dodf::QueueManager::getNumOfQueues()
{
	return m_queueCreateInfo.size();
}

VkQueue dodf::QueueManager::getGraphicsQueue()
{
	return m_graphicsQueue;
}

VkQueue dodf::QueueManager::getTransferQueue()
{
	return m_transferQueue;
}

VkQueue dodf::QueueManager::getComputeQueue()
{
	return m_computeQueue;
}

VkQueue dodf::QueueManager::getPresentQueue()
{
	return m_presentQueue;
}

const std::vector<VkDeviceQueueCreateInfo>& dodf::QueueManager::getQueueCreateInfo()
{
	return m_queueCreateInfo;
}

bool dodf::QueueManager::checkQueuesDevice(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	bool isGraphicsQueueAvailable = false;
	bool isPresentQueueAvailable = false;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			//indices.graphicsFamily = i;
			isGraphicsQueueAvailable = true;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			isPresentQueueAvailable = true;
		}

		i++;
	}

	return isPresentQueueAvailable && isGraphicsQueueAvailable;
}