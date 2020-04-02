#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "common/VulkanStructs.h"
#include "VertexDefinitions.h"
#include "Entity.h"
#include "TransformComponentMgr.h"

class MeshComponentMgr;

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();
	void cleanup();
	void createCommandBuffers(const MeshComponentMgr& meshComponentMgr);
	void createInstance(std::vector<const char*>& extensions);
	void init(const MeshComponentMgr& meshComponentMgr);
	void draw(bool framebufferResized, const vector<Entity>& entities, const TransformComponentMgr& transformComponent, const MeshComponentMgr& meshComponentMgr);

	//----getters-----
	VkInstance getInstance() const;
	VkSurfaceKHR& Surface();
private:

#ifdef NDEBUG
	const bool VALIDATION_LAYERS_ENABLED = false;
#else
	const bool VALIDATION_LAYERS_ENABLED = true;
#endif
	const std::vector<const char*> VALIDATION_LAYERS = {
		// This is a meta layer that enables all of the standard
		// validation layers in the correct order :
		// threading, parameter_validation, device_limits, object_tracker, image, core_validation, swapchain, and unique_objects
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_LUNARG_monitor",
		"VK_LAYER_LUNARG_assistant_layer"
	};
	const std::vector<const char*> DEVICE_EXTENSIONS = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	const int MAX_FRAMES_IN_FLIGHT = 2;


	VkDescriptorPool m_descriptorPool;
	std::vector<VkDescriptorSet> m_descriptorSets;
	VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;

	VkInstance m_vkInstance = VK_NULL_HANDLE;
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device = VK_NULL_HANDLE;
	//VkQueue m_graphicsQueue = VK_NULL_HANDLE;
	//VkQueue m_presentQueue = VK_NULL_HANDLE;
	VkRenderPass m_renderPass = VK_NULL_HANDLE;
	VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;

	//VkCommandPool m_commandPool = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> m_commandBuffers;

	VkImage m_depthImage = VK_NULL_HANDLE;
	VkDeviceMemory m_depthImageMemory = VK_NULL_HANDLE;
	VkImageView m_depthImageView = VK_NULL_HANDLE;

	VkImage m_textureImage = VK_NULL_HANDLE;
	VkDeviceMemory m_textureImageMemory = VK_NULL_HANDLE;
	VkImageView m_textureImageView = VK_NULL_HANDLE;
	VkSampler m_textureSampler = VK_NULL_HANDLE;

	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;

	std::vector<VkImage> m_swapChainImages;
	std::vector<VkImageView> m_swapChainImageViews;
	std::vector<VkFramebuffer> m_swapChainFramebuffers;

	//std::vector<Vertex> m_vertices;

	/*const std::vector<Vertex> m_vertices = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};	*/
	/*const std::vector<Vertex> m_vertices = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}
	};

	const std::vector<uint32_t> m_indices = {
		0, 1, 2
	};*/

	/*const std::vector<Vertex> m_vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
		{{0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}}
	};

	const std::vector<uint32_t> m_indices = {
		0, 1, 2,
		1, 3, 2
	};*/

	//VkBuffer m_vertexBuffer;
	//std::vector<VkBuffer> m_vertexBuffer;
	//VkDeviceMemory m_vertexBufferMemory;
	//std::vector<VkDeviceMemory> m_vertexBufferMemory;
	//VkBuffer m_indexBuffer;
	//std::vector<VkBuffer> m_indexBuffer;
	//VkDeviceMemory m_indexBufferMemory;
	//std::vector<VkDeviceMemory> m_indexBufferMemory;

	std::vector<VkBuffer> m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;

	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
	size_t m_currentFrame = 0;

	void addRequiredExtensions(std::vector<const char*>& extensions);
	/*VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);*/
	bool checkValidationLayerSupport();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);	
	void cleanupSwapChain();
	//void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & bufferMemory);
	//void createCommandPool();
	void createDepthResources();
	void createDescriptorPool();
	void createDescriptorSetLayout();
	void createDescriptorSets();
	void createFramebuffers();
	void createGraphicsPipeline();
	void createImage(
		uint32_t width,
		uint32_t height,
		uint32_t mipLevels,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkImage& image,
		VkDeviceMemory& imageMemory);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void createImageViews();
	//void createIndexBuffer(const MeshComponentMgr& meshComponentMgr);
	void createLogicalDevice();
	void createRenderPass();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createSyncObjects();
	void createSwapChain();
	void createTextureImage();
	void createTextureSampler();
	void createUniformBuffers();
	uint32_t m_mipLevels = 0; //pending to remove and doing it better with a texture manager.
	//void createVertexBuffer(const MeshComponentMgr& meshComponentMgr);
	VkFormat findDepthFormat();
	//QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	bool hasStencilComponent(VkFormat format);
	bool isDeviceSuitable(VkPhysicalDevice device);
	void pickPhysicalDevice();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	void recreateSwapChain(const MeshComponentMgr& meshComponentMgr);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void updateUniformBuffer(uint32_t currentImage, const glm::mat4 & model);
};
