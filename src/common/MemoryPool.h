#pragma once

#include <cstdint>

namespace dodf {
	namespace MemoryPool {
		void Initialize(size_t size);
		void Destroy();

		void* Get(size_t byteSize, size_t alignment = 8);
		namespace Debug {
			uintptr_t GetCurrentAddress();
			size_t GetCurrentSize();
		}
	}
}
