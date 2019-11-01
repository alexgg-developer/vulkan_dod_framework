#include "CommandManager.h"

#include "QueueManager.h"

namespace dodf {
	namespace CommandManager {
		namespace impl {
			VkCommandPool m_cmdGraphicsPool;
			VkCommandPool m_cmdTransferPool;
			VkCommandPool m_cmdComputePool;

			VkQueue m_graphicsQueue;
			VkQueue m_transferQueue;
			VkQueue m_computeQueue;

			VkQueue getQueueType(Pool type);
		}
	}
}

VkQueue dodf::CommandManager::impl::getQueueType(Pool type)
{
	VkQueue queue;

	switch (type) {
	case Pool::GRAPHICS:
		queue = QueueManager::getGraphicsQueue();
		break;
	case Pool::TRANSFER:
		queue = QueueManager::getTransferQueue();
		break;
	case Pool::COMPUTE:
		queue = QueueManager::getComputeQueue();
		break;
	}

	return queue;
}

using namespace dodf::CommandManager::impl;


VkCommandPool dodf::CommandManager::getPoolByType(Pool type)
{
	VkCommandPool pool;

	switch (type) {
	case Pool::GRAPHICS:
		pool = m_cmdGraphicsPool;
		break;
	case Pool::TRANSFER:
		pool = m_cmdTransferPool;
		break;
	case Pool::COMPUTE:
		pool = m_cmdComputePool;
		break;
	}

	return pool;
}

VkCommandBuffer dodf::CommandManager::createCommandBuffer(VkCommandBufferLevel level, Pool type, VkDevice device)
{
	VkCommandPool pool = getPoolByType(type);

	VkCommandBuffer cmdBuffer;
	VkCommandBufferAllocateInfo cmdBufAllocateInfo =
		commandBufferAllocateInfo(
			pool,
			level,
			1);

	VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &cmdBufAllocateInfo, &cmdBuffer));

	return cmdBuffer;
}


void dodf::CommandManager::destroy(VkDevice device)
{
	vkDestroyCommandPool(device, m_cmdGraphicsPool, nullptr);
	vkDestroyCommandPool(device, m_cmdComputePool, nullptr);
	vkDestroyCommandPool(device, m_cmdTransferPool, nullptr);
}

VkCommandBuffer dodf::CommandManager::beginSingleUseCmdBuffer(VkCommandBufferLevel level, Pool type, VkDevice device)
{
	VkCommandBuffer cmdBuffer = createCommandBuffer(level, type, device);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(cmdBuffer, &beginInfo);

	return cmdBuffer;
}

void dodf::CommandManager::endSingleUseCmdBuffer(VkCommandBuffer cmdBuffer, Pool type, VkDevice device)
{
	VK_CHECK_RESULT(vkEndCommandBuffer(cmdBuffer));

	VkSubmitInfo vkSubmitInfo = {};
	vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vkSubmitInfo.commandBufferCount = 1;
	vkSubmitInfo.pCommandBuffers = &cmdBuffer;

	// Create fence to ensure that the command buffer has finished executing
	/*VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = 0;
	VkFence fence;
	VK_CHECK_RESULT(vkCreateFence(device, &fenceCreateInfo, nullptr, &fence));*/

	//VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &vkSubmitInfo, fence));
	VK_CHECK_RESULT(vkQueueSubmit(getQueueType(type), 1, &vkSubmitInfo, VK_NULL_HANDLE));
	/*const unsigned long long _MAX_UINT_64_ = 0xffffffffffffffff;
	VK_CHECK_RESULT(vkWaitForFences(Screen::detail::device, 1, &fence, false, _MAX_UINT_64_));
	vkDestroyFence(Screen::detail::device, fence, nullptr);*/
	vkQueueWaitIdle(getQueueType(type));
	//if (free) {
	vkFreeCommandBuffers(device, getPoolByType(type), 1, &cmdBuffer);
	//}
}

VkCommandBufferAllocateInfo dodf::CommandManager::commandBufferAllocateInfo(VkCommandPool commandPool, VkCommandBufferLevel level, uint32_t bufferCount)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = level;
	commandBufferAllocateInfo.commandBufferCount = bufferCount;
	return commandBufferAllocateInfo;
}

void dodf::CommandManager::createCommandPools(VkDevice device)
{
	using namespace dodf::QueueManager;

	VkCommandPoolCreateInfo cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	cmdPoolInfo.queueFamilyIndex = getIndexQueue(Queue::GRAPHICS);
	VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &m_cmdGraphicsPool));
	size_t numTypeQueues = getNumOfQueues();
	if (numTypeQueues > 1) {
		VkCommandPoolCreateInfo cmdPoolInfoTransfer = {};
		cmdPoolInfoTransfer.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfoTransfer.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		cmdPoolInfoTransfer.queueFamilyIndex = getIndexQueue(Queue::TRANSFER);
		VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfoTransfer, nullptr, &m_cmdTransferPool));

		if (numTypeQueues > 2) {
			VkCommandPoolCreateInfo cmdPoolInfoCompute = {};
			cmdPoolInfoCompute.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			cmdPoolInfoCompute.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			cmdPoolInfoCompute.queueFamilyIndex = getIndexQueue(Queue::COMPUTE);
			VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfoCompute, nullptr, &m_cmdComputePool));
		}
		else {
			m_cmdComputePool = m_cmdGraphicsPool;
		}
	}
	else {
		m_cmdTransferPool = m_cmdGraphicsPool;
		m_cmdComputePool = m_cmdGraphicsPool;
	}

}
