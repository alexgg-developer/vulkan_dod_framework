#include <cstdlib>
#include <math.h>
#include <vector>

#include "MemoryPool.h"
#include "Assert.h"

namespace dodf {
	namespace MemoryPool {
		namespace impl {
			uintptr_t m_currentAddress;
			uintptr_t* m_pool;
			size_t m_size, m_currentSize;
		}
	}
}

using namespace dodf;
using namespace MemoryPool::impl;


void MemoryPool::Initialize(size_t size)
{
	m_pool = static_cast<uintptr_t*>(malloc(size));
	//freeNodes = static_cast<llnode_t **>(malloc(sizeof(llnode_t *) ));
	m_currentAddress = reinterpret_cast<uintptr_t>(m_pool);
	m_currentSize = 0;
	m_size = size;
}

void MemoryPool::Destroy()
{
	free(m_pool);
}

void* MemoryPool::Get(size_t byteSize, size_t alignment) 
{
	
	ASSERT((alignment & (alignment - 1)) == 0 && "alignment needs to be power of 2");

	size_t expandedSize_bytes = byteSize + alignment;
	uintptr_t rawAddress = m_currentAddress;

	size_t mask = (alignment - 1);
	uintptr_t misalignment = (rawAddress & mask);
	ptrdiff_t adjustment = alignment - misalignment;
	// Calculate the adjusted address, return as pointer.
	uintptr_t alignedAddress = rawAddress + adjustment;


	m_currentAddress = alignedAddress + expandedSize_bytes;
	m_currentSize += expandedSize_bytes + adjustment;

	ASSERT(m_size > m_currentSize && "trying to get more of what it is currently allocated");
	return reinterpret_cast<void*>(alignedAddress);
}

uintptr_t dodf::MemoryPool::Debug::GetCurrentAddress()
{
	return impl::m_currentAddress;
}

size_t dodf::MemoryPool::Debug::GetCurrentSize()
{
	return impl::m_currentSize;
}
