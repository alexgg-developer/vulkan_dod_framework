#pragma once

#include "Assert.h"
#include "IComponentManager.h"
#include <glm/glm.hpp>

#include <unordered_map>

using namespace dodf;
using namespace std;
using namespace glm;

class MeshComponentMgr: public IComponentManager 
{
private :
	struct InstanceData {
		uint32_t * indexCount;
		uint32_t * startIndexes;
		unsigned long long * meshSizeInBytes; //64 bits always
		unsigned long long * startMesh; //64 bits always (offset in bytes)
		uint64_t *gpuVertexbufferOffset, *gpuIndexBufferOffset;
	};
	InstanceData m_data;
	uint8_t *m_vertexData, *m_indexData;
	Instance* m_graphicsQueue;
	size_t m_graphicsQueueSize = 0;
public:	
	void add(const Entity& e) override;
	void allocate(size_t size) override;
	const Instance* getGraphicsQueue() const;

	//gets
	inline const uint8_t* const getVertexData() const { return m_vertexData; }
	inline const uint8_t* const getIndexData() const { return m_indexData; }
	inline const InstanceData& getInstanceData() const { return m_data; }
	//inline const Instance* getGraphicsQueue() const { return m_graphicsQueue; }
	inline size_t getGraphicsQueueSize() const { return m_graphicsQueueSize; }
	void pushDataToGPU(
		const unsigned long long vertexByteSize,
		const void * verticesData,
		const unsigned long long indexesByteSize,
		const void * indexesData,
		Instance i);

	//templates
	/*template <typename T>
	void setMesh(Instance i, const std::vector<T>& mesh, bool pushDataToGPU)
	{
		ASSERT(m_buffer.memoryUsed < m_buffer.allocated);
		m_data.startMesh[i.i] = m_buffer.n > 1 ? m_data.startMesh[i.i - 1] + m_data.meshSizeInBytes[i.i - 1] : 0;
		m_data.meshSizeInBytes[i.i] = sizeof(T) * mesh.size();
		//static_cast<uint8_t*>(m_buffer.data)[startMesh] = mesh.data();
		//memcpy(&static_cast<uint8_t*>(m_buffer.data)[m_data.startMesh[i.i]], static_cast<void*>(mesh.data()), m_data.meshSizeInBytes[i.i]);
		//auto vertexDataDest = static_cast<uint8_t*>(m_vertexData)[m_data.startMesh[i.i]];
		memcpy(&m_vertexData[m_data.startMesh[i.i]], mesh.data(), m_data.meshSizeInBytes[i.i]);
		//float r = 1.0;
		//void * vertexData = MemoryPool::Get(100);
		//memcpy(&m_vertexData[m_data.startMesh[i.i]], &r, sizeof(float));
		//memcpy(vertexData, &r, sizeof(float));
		m_buffer.memoryUsed += m_data.meshSizeInBytes[i.i];
		m_data.indexCount[i.i] = 0;
		m_graphicsQueue[m_graphicsQueueSize++] = i;
		if (pushDataToGPU) {
			pushDataToGPU(m_data.meshSizeInBytes[i.i], static_cast<const void*>mesh.data(), 0, nullptr, i);
		}
	}*/

	template <typename T>
	void setMesh(Instance i, const std::vector<T>& mesh, const std::vector<uint32_t>& indexes)
	{
		//setMesh(i, mesh);
		ASSERT(m_buffer.memoryUsed < m_buffer.allocated);
		m_data.startMesh[i.i] = m_buffer.n > 1 ? m_data.startMesh[i.i - 1] + m_data.meshSizeInBytes[i.i - 1] : 0;
		m_data.meshSizeInBytes[i.i] = sizeof(T) * mesh.size();
		//static_cast<uint8_t*>(m_buffer.data)[startMesh] = mesh.data();
		//memcpy(&static_cast<uint8_t*>(m_buffer.data)[m_data.startMesh[i.i]], static_cast<void*>(mesh.data()), m_data.meshSizeInBytes[i.i]);
		//auto vertexDataDest = static_cast<uint8_t*>(m_vertexData)[m_data.startMesh[i.i]];
		memcpy(&m_vertexData[m_data.startMesh[i.i]], mesh.data(), m_data.meshSizeInBytes[i.i]);
		//float r = 1.0;
		//void * vertexData = MemoryPool::Get(100);
		//memcpy(&m_vertexData[m_data.startMesh[i.i]], &r, sizeof(float));
		//memcpy(vertexData, &r, sizeof(float));
		m_buffer.memoryUsed += m_data.meshSizeInBytes[i.i];
		//m_data.indexCount[i.i] = 0;

		m_data.indexCount[i.i] = static_cast<uint32_t>(indexes.size());
		m_data.startIndexes[i.i] = m_buffer.n > 1 ? m_data.startIndexes[i.i - 1] + m_data.indexCount[i.i - 1] : 0;
		memcpy(&reinterpret_cast<uint32_t*>(m_indexData)[m_data.startIndexes[i.i]], indexes.data(), m_data.indexCount[i.i] * sizeof(uint32_t));
		m_buffer.memoryUsed += m_data.indexCount[i.i] * sizeof(uint32_t);

		pushDataToGPU(m_data.meshSizeInBytes[i.i], static_cast<const void*>(mesh.data()), m_data.indexCount[i.i] * sizeof(uint32_t), static_cast<const void*>(indexes.data()), i);

		m_graphicsQueue[m_graphicsQueueSize++] = i;
	}
};

