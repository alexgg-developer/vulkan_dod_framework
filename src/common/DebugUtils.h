#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef _DEBUG
	#ifdef _MSC_VER
		#define ASSERT(x) if (!(x)) __debugbreak();
	#else assert(x)
	#endif
#else
	#define ASSERT(x) assert(x)
#endif

#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	ASSERT(res == VK_SUCCESS);																									\
}	

class DebugUtils
{
private:
	static VkDebugUtilsMessengerEXT m_callback;
public:
	static VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator = nullptr, VkDebugUtilsMessengerEXT* callBack = VK_NULL_HANDLE);
	static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback = VK_NULL_HANDLE, const VkAllocationCallbacks* pAllocator = nullptr);
	static void setupDebugging(VkInstance instance);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
};
