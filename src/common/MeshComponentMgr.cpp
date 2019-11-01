#include "MeshComponentMgr.h"
#include "MemoryPool.h"
#include "MemoryPoolGPU.h"
#include "VertexDefinitions.h"

#include <glm/gtc/matrix_transform.hpp>


void MeshComponentMgr::allocate(size_t size)
{
	constexpr size_t INDEX_VERTEX_MAX_NUM = 10000;
	constexpr size_t APROX_SIZE_MESH_BYTES = INDEX_VERTEX_MAX_NUM * sizeof(TexturedVertex); //using maximum size available for vertexData
	size_t sizeVertexDataBytes = size * APROX_SIZE_MESH_BYTES;
	m_vertexData = static_cast<uint8_t*>(MemoryPool::Get(sizeVertexDataBytes));

	m_indexData = static_cast<uint8_t*>(MemoryPool::Get(size * INDEX_VERTEX_MAX_NUM * sizeof(uint32_t)));

	size_t dataBufferByteSize = size * (2 * sizeof(uint32_t) + 2 * sizeof(unsigned long long) + 2 * sizeof(uint64_t));
	m_buffer.data = MemoryPool::Get(dataBufferByteSize);
	m_buffer.allocated = dataBufferByteSize;
	m_buffer.memoryUsed = 0;
	m_buffer.n = 0;

	m_data.indexCount = static_cast<uint32_t*>(m_buffer.data);
	m_data.startIndexes = m_data.indexCount + size;
	m_data.meshSizeInBytes = reinterpret_cast<unsigned long long*>(m_data.startIndexes) + size;
	m_data.startMesh = m_data.meshSizeInBytes + size;
	m_data.gpuVertexbufferOffset = m_data.startMesh + size;
	m_data.gpuIndexBufferOffset = m_data.gpuVertexbufferOffset + size;

	m_graphicsQueue = static_cast<Instance*>(MemoryPool::Get(size * (sizeof(Instance))));
	m_graphicsQueueSize = 0;
}

const Instance * MeshComponentMgr::getGraphicsQueue() const
{
	return m_graphicsQueue;
}


void MeshComponentMgr::add(const Entity& e)
{
	auto instance = Instance::create(m_buffer.n);
	m_map[e.index()] = instance;
	++m_buffer.n;
}


void MeshComponentMgr::pushDataToGPU(
	const unsigned long long vertSize,
	const void * vertsData,
	const unsigned long long indexesSize,
	const void * indexesData,
	Instance i)
{
	MemoryPoolGPU::mapDataToBuffer(
		vertSize,
		vertsData);
	m_data.gpuVertexbufferOffset[i.i] = MemoryPoolGPU::pushBackStagingToGlobal(vertSize);

	MemoryPoolGPU::mapDataToBuffer(
		indexesSize,
		indexesData);
	m_data.gpuIndexBufferOffset[i.i] = MemoryPoolGPU::pushBackStagingToGlobal(indexesSize);
}