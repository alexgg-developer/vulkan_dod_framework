#include <cstdlib>
#include <math.h>
#include <vector>

#include "MemoryPoolGPU.h"
#include "CommandManager.h"

#include "Assert.h"

namespace dodf {
	namespace MemoryPoolGPU {
		namespace impl {
			/*uintptr_t m_currentAddress;
			uintptr_t* m_pool;
			size_t m_size, m_currentSize;*/
			VkBuffer m_gpuBuffer;
			VkBuffer m_gpuStagingBuffer;
			//VkBuffer gpuPersistentStagingBuffer;
			VkDeviceMemory m_gpuMemory;
			VkDeviceMemory m_gpuStagingMemory;
			size_t m_gpuBuffersize, m_gpuBufferCurrentSize;
			VkDevice m_device;
			VkDeviceSize m_deviceAlignment;
			VkDeviceSize m_currentOffset;
		}
	}
}

using namespace dodf;
using namespace MemoryPoolGPU::impl;


void MemoryPoolGPU::initialize(size_t size, VkDevice device, VkPhysicalDevice physicalDevice)
{

	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
	m_deviceAlignment = (
		physicalDeviceProperties.limits.minUniformBufferOffsetAlignment >
		physicalDeviceProperties.limits.minStorageBufferOffsetAlignment) ?
		physicalDeviceProperties.limits.minUniformBufferOffsetAlignment :
		physicalDeviceProperties.limits.minStorageBufferOffsetAlignment;
	m_currentOffset = 0;
	//m_deviceAlignment = alignment;
	m_device = device;

	//this alignment adds 1 to the division if there is no exact division
	//when later this is multiplied again for the alignment gives the size with some 
	//padding to fit the alignment if needed.	
	unsigned long long sizeAlign = (1024 * 1024 * size) / m_deviceAlignment;
	sizeAlign += ((1024 * 1024 * size) % m_deviceAlignment > 0) ? 1 : 0;
	m_gpuBuffersize = sizeAlign * m_deviceAlignment;
	m_gpuBufferCurrentSize = 0;

	dodf::vkUtils::createBuffer(
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		m_gpuBuffersize,
		&m_gpuBuffer,
		m_device
	);

	dodf::vkUtils::createBuffer(
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		m_gpuBuffersize,
		&m_gpuStagingBuffer,
		m_device
	);

	dodf::vkUtils::allocateMemory(
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&m_gpuBuffer,
		&m_gpuMemory,
		m_device,
		physicalDevice);

	dodf::vkUtils::allocateMemory(
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&m_gpuStagingBuffer,
		&m_gpuStagingMemory,
		m_device,
		physicalDevice);

	VK_CHECK_RESULT(vkBindBufferMemory(m_device, m_gpuBuffer, m_gpuMemory, 0));
	VK_CHECK_RESULT(vkBindBufferMemory(m_device, m_gpuStagingBuffer, m_gpuStagingMemory, 0));
}

void MemoryPoolGPU::destroy()
{
	vkFreeMemory(m_device, m_gpuMemory, nullptr);
	vkFreeMemory(m_device, m_gpuStagingMemory, nullptr);
	//vkFreeMemory(m_device, gpuPersistentStagingMemory, nullptr);

	vkDestroyBuffer(m_device, m_gpuBuffer, nullptr);
	vkDestroyBuffer(m_device, m_gpuStagingBuffer, nullptr);
	//vkDestroyBuffer(m_device, gpuPersistentStagingBuffer, nullptr);
}

VkBuffer dodf::MemoryPoolGPU::getGPUBuffer()
{
	return m_gpuBuffer;
}


void MemoryPoolGPU::mapDataToBuffer(VkDeviceSize size, const void * data)
{
	VkDeviceSize sizeGPU = (size % m_deviceAlignment == 0) ? size : size + (m_deviceAlignment - size % m_deviceAlignment); //we add padding
	void * mapped;
	VK_CHECK_RESULT(vkMapMemory(m_device, m_gpuStagingMemory, 0, sizeGPU, 0, &mapped));
	memcpy(mapped, data, size);
	vkUnmapMemory(m_device, m_gpuStagingMemory);
}

VkDeviceSize MemoryPoolGPU::pushBackStagingToGlobal(VkDeviceSize size) 
{
	VkDeviceSize resultOffset = m_currentOffset;

	VkCommandBuffer copyCmd = CommandManager::beginSingleUseCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, CommandManager::Pool::TRANSFER, m_device);

	//vkDebug::DebugMarker::BeginRegion(copyCmd, "Copy Cmd Push Backs", vec4_t{ 0.8f, 0.8f, 0.8f, 1.0f });

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = m_currentOffset;
	copyRegion.size = size;
	vkCmdCopyBuffer(copyCmd, m_gpuStagingBuffer, m_gpuBuffer, 1, &copyRegion);

	VkDeviceSize indexOffset = (size) / m_deviceAlignment;
	indexOffset += ((size) % m_deviceAlignment > 0) ? 1 : 0;
	m_currentOffset += (indexOffset * m_deviceAlignment);

	//vkDebug::DebugMarker::EndRegion(copyCmd);

	//Render::detail::FlushCommandBuffer(copyCmd, Screen::detail::transferQueue, true, &CommandManager::detail::cmdPoolTransfer);
	CommandManager::endSingleUseCmdBuffer(copyCmd, CommandManager::Pool::TRANSFER, m_device);

	return resultOffset;
}