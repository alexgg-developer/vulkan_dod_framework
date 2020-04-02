#pragma once

#include "Assert.h"
#include "IComponentManager.h"
#include "VertexDefinitions.h"
#include "IdDeclarations.h"

#include <glm/glm.hpp>

#include <unordered_map>

using namespace std;
using namespace glm;

class MeshComponentMgr: public IComponentManager 
{
private :
	static const size_t MESH_NAME_SIZE = 128;
	struct InstanceData {
		uint32_t * indexCount;
		uint32_t * startIndexes;
		unsigned long long * meshSizeInBytes; //64 bits always
		unsigned long long * startMesh; //64 bits always (offset in bytes)
		uint64_t *gpuVertexbufferOffset, *gpuIndexBufferOffset;
	};
	InstanceData m_data;
	uint8_t *m_vertexData, *m_indexData;
	char(*m_name)[MESH_NAME_SIZE];
	materialExternalId_t* m_material;

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

	template <typename T>
	void setMesh(Instance i, const Mesh<T>& mesh)
	{
		//setMesh(i, mesh);
		ASSERT(m_buffer.memoryUsed < m_buffer.allocated);
		m_data.startMesh[i.i] = m_buffer.n > 1 ? m_data.startMesh[i.i - 1] + m_data.meshSizeInBytes[i.i - 1] : 0;
		m_buffer.memoryUsed += sizeof(unsigned long long);
		m_data.meshSizeInBytes[i.i] = sizeof(T) * mesh.vertices.size();
		m_buffer.memoryUsed += sizeof(unsigned long long);
		memcpy(&m_vertexData[m_data.startMesh[i.i]], mesh.vertices.data(), m_data.meshSizeInBytes[i.i]);

		m_data.indexCount[i.i] = static_cast<uint32_t>(mesh.indexes.size());
		m_buffer.memoryUsed += sizeof(uint32_t);
		m_data.startIndexes[i.i] = m_buffer.n > 1 ? m_data.startIndexes[i.i - 1] + m_data.indexCount[i.i - 1] : 0;
		m_buffer.memoryUsed += sizeof(uint32_t);
		memcpy(&reinterpret_cast<uint32_t*>(m_indexData)[m_data.startIndexes[i.i]], mesh.indexes.data(), m_data.indexCount[i.i] * sizeof(uint32_t));		
		memcpy(m_name[i.i], mesh.name.data(), MESH_NAME_SIZE * sizeof(char) );

		pushDataToGPU(m_data.meshSizeInBytes[i.i], 
			static_cast<const void*>(mesh.vertices.data()), 
			m_data.indexCount[i.i] * sizeof(uint32_t), 
			static_cast<const void*>(mesh.indexes.data()), 
			i);

		m_graphicsQueue[m_graphicsQueueSize++] = i;
	}
};

