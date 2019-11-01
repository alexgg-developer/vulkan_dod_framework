#pragma once


#include "VulkanUtils.h"

namespace dodf {
	namespace MemoryPoolGPU {
		void destroy();
		VkBuffer getGPUBuffer();
		//maps to staging memory
		void mapDataToBuffer(VkDeviceSize size, const void * data);
		//push data to the graphics card buffer, returns offset to data
		VkDeviceSize pushBackStagingToGlobal(VkDeviceSize size);
		//size in MB
		void initialize(size_t size, VkDevice device, VkPhysicalDevice physicalDevice);
		/*void* get(size_t byteSize, size_t alignment = 8);
		namespace Debug {
			uintptr_t GetCurrentAddress();
			size_t GetCurrentSize();
		}*/
	}
}
