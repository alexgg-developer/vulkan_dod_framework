#include "VulkanUtils.h"
#include "CommandManager.h"
#include "FileUtils.h"
#include "Assert.h"

/*void vkUtils::createImage(
	uint32_t width, 
	uint32_t height, 
	VkFormat format, 
	VkImageTiling tiling, 
	VkImageUsageFlags usage, 
	VkMemoryPropertyFlags properties, 
	VkImage& image, 
	VkDeviceMemory& imageMemory) 
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(device, image, imageMemory, 0);
}*/

void dodf::vkUtils::createBuffer(
	VkBufferUsageFlags usageFlags, 
	VkDeviceSize size, 
	VkBuffer * buffer,
	VkDevice device) 
{
	
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.pNext = NULL;
	bufferCreateInfo.usage = usageFlags;
	bufferCreateInfo.size = size;
	bufferCreateInfo.flags = 0; 
	VK_CHECK_RESULT(vkCreateBuffer(device, &bufferCreateInfo, nullptr, buffer));
}

void dodf::vkUtils::allocateMemory(
	VkMemoryPropertyFlags memoryPropertyFlags, 
	VkBuffer * buffer, 
	VkDeviceMemory * memory,
	VkDevice device,
	VkPhysicalDevice physicalDevice) 
{
	VkMemoryAllocateInfo memAllocInfo = {};
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.pNext = NULL;
	memAllocInfo.allocationSize = 0;
	memAllocInfo.memoryTypeIndex = 0;
	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(device, *buffer, &memReqs);
	memAllocInfo.allocationSize = memReqs.size;
	memAllocInfo.memoryTypeIndex = dodf::vkUtils::findMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags, physicalDevice);
	VK_CHECK_RESULT(vkAllocateMemory(device, &memAllocInfo, nullptr, memory));
}

uint32_t dodf::vkUtils::findMemoryType(
	uint32_t typeFilter, 
	VkMemoryPropertyFlags properties,
	VkPhysicalDevice physicalDevice)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	ASSERT(false && "failed to find suitable memory type!");
	return UINT32_MAX;
}

void dodf::vkUtils::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkDevice device)
{
	//VkCommandBuffer commandBuffer = beginSingleTimeCommands();
	//VkCommandBuffer commandBuffer = CommandManager::beginSingleUseCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, pool, m_device);
	//TO DO: Check if transfer is ok
	auto poolType = CommandManager::Pool::TRANSFER;
	VkCommandBuffer commandBuffer = CommandManager::beginSingleUseCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, poolType, device);
	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};
	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	//endSingleTimeCommands(commandBuffer);
	CommandManager::endSingleUseCmdBuffer(commandBuffer, poolType, device);
}

std::vector<std::vector<VkDescriptorSetLayoutBinding>> dodf::vkUtils::reflectDescriptors(const std::string & shaderFilePath, const VkShaderStageFlags & shaderStageFlags)
{
	// Generate reflection data for the shader
	auto shaderCode = FileUtils::readFile(shaderFilePath);
	SpvReflectShaderModule module;
	SpvReflectResult result = spvReflectCreateShaderModule(shaderCode.size(), shaderCode.data(), &module);
	ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

	uint32_t numDescriptorSets = 0;

	spvReflectEnumerateDescriptorSets(&module, &numDescriptorSets, NULL);
	ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

	std::vector<std::vector<VkDescriptorSetLayoutBinding>> descriptorSets(numDescriptorSets);
	for (unsigned int i = 0; i < numDescriptorSets; i++)
	{
		//descriptors[i] = {};
		//descriptors[i].numBindings = module.descriptor_sets[i].binding_count;
		std::vector<VkDescriptorSetLayoutBinding> bindings(module.descriptor_sets[i].binding_count);

		for (unsigned int j = 0; j < module.descriptor_sets[i].binding_count; j++)
		{
			bindings[j] = VkDescriptorSetLayoutBinding{
				module.descriptor_sets[i].bindings[j]->binding,
				module.descriptor_sets[i].bindings[j]->descriptor_type,
				1,
				shaderStageFlags,
				NULL
			};
		}
		descriptorSets[i] = bindings;
	}
	spvReflectDestroyShaderModule(&module);

	return descriptorSets;
}
